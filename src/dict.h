//
// Created by lxm on 5/10/21.
//
#include <stdlib.h>
#include <stdint-gcc.h>

#ifndef REDIS_STUDY_DICT_H
#define REDIS_STUDY_DICT_H

#endif //REDIS_STUDY_DICT_H

#define DICT_OK  0
#define DICT_ERR 1
#define DICT_DEFAULT_LOAD_FACTOR 0.8
#define DICT_REHASH_DONE 0
#define DICT_REHASHING 1
#define DICT_HASH_KEY(d, k) (d)->type->hashFunction(k)
#define DICTHT_DEFAULT_INIT_SIZE 4

typedef struct dictEntry {
    void *k;
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
    struct dictEntry *next;
} dictEntry;

typedef struct dictType {
    unsigned int (*hashFunction)(const void *key);
    int (*keyCompare)(const void *key1, const void *key2);
    int (*keyDestructor)(const void *key);
    int (*valDestructor)(const void *val);
} dictType;

typedef struct dictht {
    dictEntry **table;
    unsigned int size;
    unsigned int sizemask;
    unsigned int used;
} dictht;

typedef struct dict {
    dictType *type;
    dictht ht[2];
    int rehashidx;
} dict;

unsigned int dictGenHashFunction(const void *key, int len);

dictType *newDictType(
        unsigned int (*hashFunction)(const void *),
        int (*keyCompare)(const void *, const void *),
        int (*keyDestructor)(const void *),
        int (*valDestructor)(const void *)
);

dict *newDict(dictType *type);
int dictInsert(dict *dc, void *key, void *val);
int dictFind(dict *dc, void *key);
int dictDelete(dict *dc, void *key);
