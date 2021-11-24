
#include <stdio.h>
#include <time.h>
#include "../src/dict.h"
#include "../src/sds.h"

static uint32_t dict_hash_function_seed = 5381;

/* MurmurHash2, by Austin Appleby
 * Note - This code makes a few assumptions about how your machine behaves -
 * 1. We can read a 4-byte value from any address without crashing
 * 2. sizeof(int) == 4
 *
 * And it has a few limitations -
 *
 * 1. It will not work incrementally.
 * 2. It will not produce the same results on little-endian and big-endian
 *    machines.
 */
unsigned int dictGenHashFn(const void *key, int len) {
    /* 'm' and 'r' are mixing constants generated offline.
     They're not really 'magic', they just happen to work well.  */
    uint32_t seed = dict_hash_function_seed;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    /* Initialize the hash to a 'random' value */
    uint32_t h = seed ^ len;

    /* Mix 4 bytes at a time into the hash */
    const unsigned char *data = (const unsigned char *) key;

    while (len >= 4) {
        uint32_t k = *(uint32_t *) data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    /* Handle the last few bytes of the input array  */
    switch (len) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };

    /* Do a few final mixes of the hash to ensure the last few
     * bytes are well-incorporated. */
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int) h;
}

typedef struct IntEntry {
    int val;
} intentry;


int destructor(const void * key) {
//    free((intentry*)key);
    return 0;
}

unsigned int hash(const void *key) {
    return dictGenHashFn(key, sizeof(intentry));
}

int keyCompare(const void * key1, const void * key2) {
    intentry* k1 = (intentry*) key1;
    intentry* k2 = (intentry*) key2;
    return k1->val == k2->val;
}

void run() {
    dictType* dtype = newDictType(hash, keyCompare, destructor, destructor);
    dict *d = newDict(dtype);

    int size = 1000000;
    intentry** entries = malloc(sizeof(void*) * size);
    for (int i = 0; i < size; i++) {
        entries[i] = malloc(sizeof(intentry));
        entries[i]->val = i;
    }

    clock_t start = clock();
    for (int i = 0; i < size; i++) {
        dictInsert(d, entries[i], entries[i]);
        dictEntry *entry = dictFind(d, entries[i]);
        if (entry == NULL) {
            printf("cannot find entry %d", i);
            exit(1);
        }
    }

    for (int i = 0; i < size; i++) {
        dictDelete(d, entries[i]);
        dictEntry *entry = dictFind(d, entries[i]);
        if (entry != NULL) {
            printf("find entry %d after delete", i);
            exit(1);
        }
    }

    double duration = 1000000 * (double)(clock() - start) / CLOCKS_PER_SEC / size;
    printf("duration: %f us", duration);
}

int main() {
    run();
    return 0;
}