//#include <stdio.h>
//#include <time.h>
//#include <malloc.h>
//#include "../src/redisdict.h"
//#include "../src/sds.h"
//
//typedef struct IntEntry {
//    int val;
//} intentry;
//
//
//int destructor(const void * key) {
//    free((intentry*)key);
//    return 0;
//}
//
//unsigned int hash(const void *key) {
//    return dictGenHashFunction(key, sizeof(intentry));
//}
//
//int keyCompare(void *privdata, const void * key1, const void * key2) {
//    intentry* k1 = (intentry*) key1;
//    intentry* k2 = (intentry*) key2;
//    return k1->val == k2->val;
//}
//
//void run() {
//    int j = 0;
//    dictType* dtype = malloc(sizeof(*dtype));
//    dtype->keyCompare = keyCompare;
//    dtype->hashFunction = hash;
//    dict *d = dictCreate(dtype, &j);
//
//    int size = 1000000;
//    intentry** entries = malloc(sizeof(void*) * size);
//    for (int i = 0; i < size; i++) {
//        entries[i] = malloc(sizeof(intentry));
//        entries[i]->val = i;
//    }
//
//    clock_t start = clock();
//    for (int i = 0; i < size; i++) {
//        dictAdd(d, entries[i], entries[i]);
//    }
//    double duration = 1000000 * (double)(clock() - start) / CLOCKS_PER_SEC / size;
//    printf("duration: %f us", duration);
//}
//
//int main() {
//    run();
//    return 0;
//}