
#include <math.h>
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
unsigned int dictGenHashFunction(const void *key, int len) {
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

int intDestructor(const void * key) {
//    sds k = (sds) key;
//    sdsfree(k);
    return 0;
}

unsigned int hash(const void * key) {
    return dictGenHashFunction(key, 5);
}

int keyCompare(const void * key1, const void * key2) {
    int *k1 = (int*) key1;
    int *k2 = (int*) key2;
    return *k1 - *k2;
}

int main() {
    dictType* dtype = newDictType(hash, keyCompare, intDestructor, intDestructor);
    dict *d = newDict(dtype);

    clock_t start_time = clock();
    for (int i = 0; i < 10000; i++) {
        dictInsert(d, (void *)&i, (void *)&i);
    }
    clock_t elapsed_time = clock() - start_time;
    printf("Done in %ld us\n", elapsed_time);
    return 0;
}