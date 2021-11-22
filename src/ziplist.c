#include <malloc.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "ziplist.h"

#define ZIP_STR_06B (1 << 6)
#define ZIP_STR_14B (1 << 14)
#define ZIP_STR_32B 0xFFFFFFFF
#define ZIP_INT_16B (0xc0 | 0<<4)
#define ZIP_INT_32B (0xc0 | 1<<4)
#define ZIP_INT_64B (0xc0 | 2<<4)
#define ZIP_INT_24B (0xc0 | 3<<4)
#define ZIP_INT_8B 0xfe
/* 4 bit integer immediate encoding */
#define ZIP_INT_IMM_MASK 0x0f
#define ZIP_INT_IMM_MIN 0xf1    /* 11110001 */
#define ZIP_INT_IMM_MAX 0xfd    /* 11111101 */

#define INT24_MAX 0x7fffff
#define INT24_MIN (-INT24_MAX - 1)

#define ZIPLIST_HEADER_SIZE sizeof(uint32_t) * 2 + sizeof(uint16_t)
#define ZIPLIST_END 255
#define ZIPLIST_DATA_LEN 254

#define ZIPLIST_STR_MASK 0xc0

#define ZIPLIST_BYTES(zl) (*(uint32_t*) zl)
#define ZIPLIST_TAIL_OFFSET(zl) (*((uint32_t*) zl + 1))
#define ZIPLIST_LEN(zl) (*((uint16_t*)((uint32_t*) zl + 2)))
#define ZIPLIST_TAIL_ENTRY(zl) (zl + ZIPLIST_TAIL_OFFSET(zl))
#define ZIPLIST_END_ENTRY(zl) (zl + ZIPLIST_BYTES(zl) - 1)
#define ZIPLIST_HEAD_ENTRY(zl) (zl + ZIPLIST_HEADER_SIZE)
#define ZIPLIST_DECODE_PREVLEN(entry, prevLength) do {               \
    uint8_t* v = (uint8_t*) entry;                                   \
    if (*v == ZIPLIST_DATA_LEN) memcpy(prevLength, entry + 1, 4);    \
    else *prevLength = ZIPLIST_DATA_LEN;                             \
} while(0);
#define ZIPLIST_PREVLEN_DIFF(reqlen, p) (reqLen - ZIPLIST_ENTRY_PREVLEN(p))
#define ZIPLIST_ENTRY_PREVLEN(p) *p[0] == ZIPLIST_DATA_LEN ? *((uint32_t*)(p + 1)) : (uint32_t)(*(uint8_t*)p)
#define ZIP_IS_STR(encoding) ((*(uint8_t*)encoding) & ZIPLIST_STR_MASK) < ZIPLIST_STR_MASK

/* Convert a string into a long long. Returns 1 if the string could be parsed
 * into a (non-overflowing) long long, 0 otherwise. The value will be set to
 * the parsed value when appropriate. */
int string2ll(const char *s, size_t slen, long long *value) {
    const char *p = s;
    size_t plen = 0;
    int negative = 0;
    unsigned long long v;

    if (plen == slen)
        return 0;

    /* Special case: first and only digit is 0. */
    if (slen == 1 && p[0] == '0') {
        if (value != NULL) *value = 0;
        return 1;
    }

    if (p[0] == '-') {
        negative = 1;
        p++; plen++;

        /* Abort on only a negative sign. */
        if (plen == slen)
            return 0;
    }

    /* First digit should be 1-9, otherwise the string should just be 0. */
    if (p[0] >= '1' && p[0] <= '9') {
        v = p[0]-'0';
        p++; plen++;
    } else if (p[0] == '0' && slen == 1) {
        *value = 0;
        return 1;
    } else {
        return 0;
    }

    while (plen < slen && p[0] >= '0' && p[0] <= '9') {
        if (v > (ULLONG_MAX / 10)) /* Overflow. */
            return 0;
        v *= 10;

        if (v > (ULLONG_MAX - (p[0]-'0'))) /* Overflow. */
            return 0;
        v += p[0]-'0';

        p++; plen++;
    }

    /* Return if not all bytes were used. */
    if (plen < slen)
        return 0;

    if (negative) {
        if (v > ((unsigned long long)(-(LLONG_MIN+1))+1)) /* Overflow. */
            return 0;
        if (value != NULL) *value = -v;
    } else {
        if (v > LLONG_MAX) /* Overflow. */
            return 0;
        if (value != NULL) *value = v;
    }
    return 1;
}

void ziplistMeta(const unsigned char* zl) {
    printf("ZIPLIST_BYTES: %d\n", ZIPLIST_BYTES(zl));
    printf("ZIPLIST_TAIL_OFFSET: %d\n", ZIPLIST_TAIL_OFFSET(zl));
    printf("ZIPLIST_LEN: %d\n", ZIPLIST_LEN(zl));
}

/*
 * ziplist bytes representation:
 *      ziplistBytes (4) | ziplistTailOffset (4) | ziplistLen (2) | entry | ...
 */
unsigned char *ziplistNew(void) {
    unsigned int bytes = ZIPLIST_HEADER_SIZE + 1;
    unsigned char* zl = malloc(ZIPLIST_HEADER_SIZE);

    ZIPLIST_BYTES(zl) = bytes;
    ZIPLIST_TAIL_OFFSET(zl) = ZIPLIST_HEADER_SIZE;
    ZIPLIST_LEN(zl) = 0;
    zl[ZIPLIST_HEADER_SIZE] = ZIPLIST_END;
    return zl;
}

typedef struct Entry {

} Entry;

static int tryEncodeInt(unsigned char *data, unsigned int dataLen, unsigned char *encoding, long long *v) {
    long long value = 0;
    // the data len cannot be represented by 64 bit int
    if (dataLen > 8) return 0;
    if (string2ll((char*)data, dataLen, &value)) {
        /* Great, the string can be encoded. Check what's the smallest
         * of our encoding types that can hold this value. */
        if (value >= 0 && value <= 12) {
            *encoding = ZIP_INT_IMM_MIN+value;
        } else if (value >= INT8_MIN && value <= INT8_MAX) {
            *encoding = ZIP_INT_8B;
        } else if (value >= INT16_MIN && value <= INT16_MAX) {
            *encoding = ZIP_INT_16B;
        } else if (value >= INT24_MIN && value <= INT24_MAX) {
            *encoding = ZIP_INT_24B;
        } else if (value >= INT32_MIN && value <= INT32_MAX) {
            *encoding = ZIP_INT_32B;
        } else {
            *encoding = ZIP_INT_64B;
        }
        *v = value;
        return 1;
    }
    return 0;
}

/* Return bytes needed to store integer encoded by 'encoding' */
static unsigned int zipIntSize(unsigned char encoding) {
    switch(encoding) {
        case ZIP_INT_8B:  return 1;
        case ZIP_INT_16B: return 2;
        case ZIP_INT_24B: return 3;
        case ZIP_INT_32B: return 4;
        case ZIP_INT_64B: return 8;
        default: return 0; /* 4 bit immediate */
    }
}

static unsigned int zipEncodingLength(unsigned char *p, unsigned int dataLen, unsigned char encoding) {
    unsigned char len = 1, buf[5];

    if (ZIP_IS_STR(&encoding)) {
        if (dataLen < 0x3f) {
            if (!p) return len;
            buf[0] = dataLen & 0xff;
        }
        else if (dataLen <= 0x3fff) {
            len = 2;
            if (!p) return len;
            buf[0] = 0x40 | (dataLen >> 8 & 0x3f);
            buf[1] = dataLen & 0xff;
        } else {
            len = 5;
            if (!p) return len;
            buf[0] = 0x80;
            buf[4] = dataLen & 0xff;
            buf[3] = (dataLen >> 8 & 0xff);
            buf[2] = (dataLen >> 16 & 0xff);
            buf[1] = (dataLen >> 24 & 0xff);
        }
    } else {
        if (!p) return len;
        buf[0] = encoding;
    }

    memcpy(p, buf, len);
    return len;
}

static unsigned int zipPrevEncodeLength(unsigned char *p, unsigned int len) {
    if (p == NULL) {
        return (len < ZIPLIST_DATA_LEN) ? 1 : sizeof(len)+1;
    } else {
        if (len < ZIPLIST_DATA_LEN) {
            p[0] = len;
            return 1;
        } else {
            p[0] = ZIPLIST_DATA_LEN;
            memcpy(p+1,&len,sizeof(len));
            return 1+sizeof(len);
        }
    }
}

/*
 * TODO: actually when to use MACRO and when to use function?
 */
static int zipPrevLenByteDiff(unsigned char* entry, unsigned int reqlen) {
    unsigned int prevlen;
    ZIPLIST_DECODE_PREVLEN(entry, &prevlen);
    return zipPrevEncodeLength(NULL, prevlen) - reqlen;
}

static unsigned char* ziplistResize(unsigned char *zl, unsigned int newlen) {
    zl = realloc(zl, newlen);
    ZIPLIST_BYTES(zl) = newlen;
    zl[newlen-1] = ZIPLIST_END;
    return zl;
}

/*
 * Each entry is as follows:
 *      prevEntryLength | encoding | dataLen | data
 */
static unsigned char *insertZiplist(unsigned char *zl, unsigned char *data, unsigned int datalen, unsigned char *p) {
    unsigned int prevLength = 0, curlen = ZIPLIST_BYTES(zl), reqlen;
    // determine the previous length
    if (p[0] != ZIPLIST_END) {
        ZIPLIST_DECODE_PREVLEN(p, &prevLength);
    }

    // determine the encoding
    unsigned char encoding = 0;
    long long value = 123456789;
    if (tryEncodeInt(data, datalen, &encoding, &value)) {
        reqlen = zipIntSize(encoding);
    } else {
        reqlen = datalen;
    }

    // determine and append the length
    reqlen += sizeof(prevLength) + 1;
    reqlen += zipEncodingLength(data, datalen, encoding);

    // ensure the insertion location can hold the previous length of the new item
    unsigned int preLengthDiff = p[0] == ZIPLIST_END ? 0 : zipPrevLenByteDiff(p, reqlen);

    unsigned int offset = p-zl;
    zl = ziplistResize(zl, curlen + reqlen + preLengthDiff);
    p = zl + offset;

    // move the data accordingly
    if (p[0] != ZIPLIST_END) {
        // move the data after prelen
        memmove(p + reqlen, p-preLengthDiff, curlen - offset - 1 + preLengthDiff);
        // encode the reqlen to the prelen of entry
        zipPrevEncodeLength(p+reqlen,reqlen);
        ZIPLIST_TAIL_OFFSET(zl) = ZIPLIST_TAIL_OFFSET(zl) + reqlen;
    } else {
        ZIPLIST_TAIL_OFFSET(zl) = p-zl;
    }

    if (preLengthDiff != 0) {
        // we need to cascade update as prelen as changed
    }

    // write the entry

    return zl;
}

unsigned char *ziplistPush(unsigned char *zl, unsigned char *s, unsigned int slen, int where) {
    unsigned char *p = where == ZIPLIST_END ? p = ZIPLIST_END_ENTRY(zl) : ZIPLIST_HEAD_ENTRY(zl);
    return insertZiplist(zl, s, slen, p);
}