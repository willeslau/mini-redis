//
//#include <stdbool.h>
//#include "dict.h"
//#include "sds.h"
//
//#define max(a, b) (((a) >= (b)) ? (a) : (b))
//#define dictIsRehashing(d) ((d)->rehashidx != -1)
//#define dictCompareKeys(d, key1, key2) \
//    (((d)->type->keyCompare) ? \
//        (d)->type->keyCompare((d)->privdata, key1, key2) : \
//        (key1) == (key2))
//
//
///* Reset an hashtable already initialized with ht_init().
// * NOTE: This function should only called by ht_destroy(). */
//static void _dictReset(dictht *ht) {
//    ht->table = NULL;
//    ht->size = 0;
//    ht->sizemask = 0;
//    ht->used = 0;
//}
//
//static void _initDict(dict *dc, dictType *type, void *privDataPtr) {
//    _dictReset(&dc->ht[0]);
//    _dictReset(&dc->ht[1]);
//    dc->type = type;
//    dc->privdata = privDataPtr;
//}
//
//dict *newDict(dictType *type, void *privDataPtr) {
//    struct dict *dc = malloc(sizeof(dict));
//    if (dc == NULL) { return NULL; }
//
//    _initDict(dc, type, privDataPtr);
//    return dc;
//}
//
//dictEntry *newDictEntry(void *key, void *val) {
//    dictEntry *entry = malloc(sizeof(dictEntry));
//    if (entry == NULL) { return NULL; }
//
//    entry->k = key;
//    entry->v.val = val;
//    entry->next = NULL;
//
//    return entry;
//}
//
//static bool _requireExpansion(dictht *ht) {
//    if (ht->size == 0) { return true; }
//    double loadFactor = (double) ht->used / ht->size;
//    return loadFactor > DICT_DEFAULT_LOAD_FACTOR;
//}
//
///*
// * If we look at this implementation, it not only performs expansion but also deduces
// * the size of for the new hashtable. We can make this simpler by making it just
// * perform expansion.
// */
//static int _dictExpand_notSoGood(dict *dc, unsigned int minSize) {
//    dc->rehashidx = DICT_REHASHING;
//    for (int i = 0; i <= 1; i++) {
//        dictht *oldTable = &dc->ht[i];
//        unsigned int size = max(oldTable->size * 2, minSize);
//
//        dictEntry **newEntries = malloc(sizeof(dictEntry) * size);
//        if (newEntries == NULL) return DICT_ERR;
//        for (unsigned int j = 0; j < size; j++) { newEntries[j] = NULL; }
//
//        oldTable->size = size;
//        unsigned int sizemask = size - 1;
//        for (unsigned int j = 0; j < oldTable->used; j++) {
//            unsigned int hash = DICT_HASH_KEY(dc, oldTable->table[j]);
//            unsigned int index = hash & sizemask;
//            newEntries[index] = oldTable->table[j];
//        }
//        dc->ht[i].table = newEntries;
//        dc->ht[i].sizemask = sizemask;
//        dc->ht[i].used = oldTable->used;
//        dc->ht[i].size = size;
//    }
//    dc->rehashidx = DICT_REHASH_DONE;
//    return DICT_OK;
//}
//
//static unsigned int _normalizePower(unsigned int n) {
//    unsigned int s = 1;
//    while (s < n) s *= 2;
//    return s;
//}
//
//static int _dictExpand(dict *dc, unsigned int size) {
//    unsigned int realsize = _normalizePower(size);
//
//    dictEntry **newEntries = malloc(sizeof(dictEntry) * realsize);
//    if (newEntries == NULL) return DICT_ERR;
//
//    unsigned int h, idx;
//    dictht *oldTable = &dc->ht[0];
//
//    dc->ht[0].size = realsize;
//    dc->ht[0].used = oldTable->used;
//    dc->ht[0].sizemask = realsize - 1;
//    dc->ht[0].table = newEntries;
//
//    for (unsigned int j = 0; j < oldTable->size; j++) {
//        if (oldTable->table[j] == NULL) { continue; }
//        h = DICT_HASH_KEY(dc, oldTable->table[j]->k);
//        idx = h & dc->ht[0].sizemask;
//        newEntries[idx] = oldTable->table[j];
//    }
//    return DICT_OK;
//}
//
//int dictExpand(dict *dc) {
//    if (dc->ht[0].size == 0) { return _dictExpand(dc, DICTHT_DEFAULT_INIT_SIZE); }
//
//    double lf = (double) dc->ht[0].used / dc->ht[0].size;
//    if (lf <= DICT_DEFAULT_LOAD_FACTOR) return DICT_OK;
//
//    return _dictExpand(dc, dc->ht[0].size * 2);
//}
//
//static int _dictKeyIndex(dict *dc, void *key) {
//    if (dictExpand(dc) == DICT_ERR) return -1;
//
//    unsigned int h, idx, table;
//
//    h = DICT_HASH_KEY(dc, key);
//    for (table = 0; table <= 1; table++) {
//        idx = h & dc->ht[table].sizemask;
//        dictEntry *e = dc->ht[table].table[idx];
//        while (e) {
//            if (dictCompareKeys(dc, key, e->k) == 0)
//                return -1;
//            e = e->next;
//        }
//        if (!dictIsRehashing(dc)) { break; }
//    }
//
//    return idx;
//}
//
//int dictInsert(dict *dc, void *key, void *val) {
//    dictEntry *entry = newDictEntry(key, val);
//    if (entry == NULL) return DICT_ERR;
//
//    unsigned int index = _dictKeyIndex(dc, key);
//    if (index == -1) return DICT_ERR;
//
//    dc->ht[0].used++;
//    entry->next = dc->ht[0].table[index];
//    dc->ht[0].table[index] = entry;
//
//    return DICT_OK;
//}
//
//void dictGet(dict *dc, void *key, void *val) {
//    unsigned int index = _dictKeyIndex(dc, key);
//
//    dictEntry *entry = newDictEntry(key, val);
//    if (entry == NULL) return;
//
//    dc->ht[0].used++;
//    entry->next = dc->ht[0].table[index];
//    dc->ht[0].table[index] = entry;
//}
//

#include <time.h>
#include "dict.h"

#define DICTHT_DEFAULT_SIZE 16
#define DICTHT_DEFAULT_REHASHING -1
#define DICTHT_DEFAULT_REHASHING_MILLIS 10
#define DICTHT_LOADFACTOR_LO 0.3
#define DICTHT_LOADFACTOR_HI 0.75
#define dicthtEntryKeyHash(ht, type, entry) type->hashFunction(entry->k) & ht->sizemask
#define dicthtIsOccupied(ht, idx) ht->table[idx] != NULL
#define dicthtGet(ht, idx) ht->table[idx]
#define dicthtSet(ht, idx, entry) do {    \
    entry->next = dicthtGet(ht, idx);     \
    ht->table[idx] = entry;               \
} while(0)
#define dicthtIncrSize(ht) ht->used++
#define dicthtLoadFactor(ht) (double) ht->used / ht->size
#define dictIsRehashing(dc) dc->rehashidx != DICTHT_DEFAULT_REHASHING
#define dictGetTimeDiff(start) (int)(clock() - start) / 1000
#define dictSwapHt(dc) do {    \
    dictht tmp = dc->ht[0];    \
    dc->ht[0] = dc->ht[1];     \
    dc->ht[1] = tmp;           \
} while(0)

dictType *newDictType(
        unsigned int (*hashFunction)(const void *),
        int (*keyCompare)(const void *, const void *),
        int (*keyDestructor)(const void *),
        int (*valDestructor)(const void *)
) {
    dictType* dtype = malloc(sizeof(*dtype));
    dtype->hashFunction = hashFunction;
    dtype->keyCompare = keyCompare;
    dtype->keyDestructor = keyDestructor;
    dtype->valDestructor = valDestructor;
    return dtype;
}


dictEntry *newDictEntry(void *key, void *val) {
    dictEntry *e = malloc(sizeof(*e));
    if (e == NULL) return NULL;
    e->k = key;
    e->v.val = val;
    return e;
}

static unsigned int normalizePower(unsigned int n) {
    unsigned int s = 1;
    while (s < n) s *= 2;
    return s;
}

static int resetDictht(dictht* ht, unsigned int size) {
    ht->size = normalizePower(size);
    ht->sizemask = ht->size-1;
    ht->used = 0;
    if (ht->table == NULL) ht->table = malloc(sizeof(dictEntry) * ht->size);
    else ht->table = realloc(ht->table, sizeof(dictEntry) * ht->size);
    return DICT_OK;
}

static int dicthtRehashInsert(dictht *ht, dictType *type, dictEntry *entry) {
    if (entry == NULL) return DICT_OK;
    unsigned int idx = dicthtEntryKeyHash(ht, type, entry);
    dictEntry *next;
    while (entry != NULL) {
        next = entry->next;
        dicthtSet(ht, idx, entry);
        entry = next;
        dicthtIncrSize(ht);
    }
    return DICT_OK;
}

static int dicthtInsert(dictht *ht, dictType *type, dictEntry *entry) {
    unsigned int idx = dicthtEntryKeyHash(ht, type, entry);
    if (dicthtIsOccupied(ht, idx)) {
        dictEntry *next = dicthtGet(ht, idx);
        while (next != NULL) {
            if (type->keyCompare(next->k, entry->k) == 0)
                return DICT_ERR;
            next = next->next;
        }
    }
    dicthtSet(ht, idx, entry);
    dicthtIncrSize(ht);
    return DICT_OK;
}

static int rehashMillis(dict *dc, int millis) {
    clock_t start = clock();
    dictht* h0 = &dc->ht[0];
    dictht* h1 = &dc->ht[1];
    do {
        dictEntry *entry = h0->table[dc->rehashidx];
        dicthtRehashInsert(h1, dc->type, entry);
        dc->rehashidx++;
    } while (dictGetTimeDiff(start) < millis && h1->used < h0->used);

    if (h0->used == h1->used) {
        dc->rehashidx = DICTHT_DEFAULT_REHASHING;
        dictSwapHt(dc);
    }
    return DICT_OK;
}

static int expandDictht(dict *dc) {
    resetDictht(&dc->ht[1], dc->ht[0].size*2);
    dc->rehashidx = 0;
    return rehashMillis(dc, DICTHT_DEFAULT_REHASHING_MILLIS);
}

static int shrinkDictht(dict *dc) {
    resetDictht(&dc->ht[1], dc->ht[0].size/2);
    dc->rehashidx = 0;
    return rehashMillis(dc, DICTHT_DEFAULT_REHASHING_MILLIS);
}

static int resizeIfNeeded(dict *dc) {
    if (dictIsRehashing(dc)) return rehashMillis(dc, DICTHT_DEFAULT_REHASHING_MILLIS);

    dictht *ht= &dc->ht[0];
    double lf = dicthtLoadFactor(ht);
    if (lf > DICTHT_LOADFACTOR_HI) return expandDictht(dc);
//    if (lf < DICTHT_LOADFACTOR_LO) return shrinkDictht(dc);
    return DICT_OK;
}

dict *newDict(dictType *type) {
    dict *d = malloc(sizeof(*d));
    if (d == NULL) return NULL;

    d->type = type;
    d->rehashidx = DICTHT_DEFAULT_REHASHING;
    resetDictht(&d->ht[0], DICTHT_DEFAULT_SIZE);
    return d;
}

int dictInsert(dict *dc, void *key, void *val) {
    dictEntry *entry = newDictEntry(key, val);
    if (entry == NULL) return DICT_ERR;
    resizeIfNeeded(dc);
    dicthtInsert(&dc->ht[0], dc->type, entry);
    return DICT_OK;
}