#include "dict.h"

#define DICTHT_DEFAULT_SIZE 16
#define DICTHT_DEFAULT_REHASHING -1
#define DICTHT_DEFAULT_REHASHING_LOOPS 5
#define DICTHT_LOADFACTOR_LO 0.3
#define DICTHT_LOADFACTOR_HI 5
#define dicthtEntryKeyIndex(ht, type, k) type->hashFunction(k) & ht->sizemask
#define dicthtIsOccupied(ht, idx) ht->table[idx] != NULL
#define dicthtGet(ht, idx) ht->table[idx]
#define dicthtSet(ht, idx, entry) do {    \
    entry->next = dicthtGet(ht, idx);     \
    ht->table[idx] = entry;               \
} while(0)
#define dicthtIncrSize(ht) ht->used++
#define dicthtLoadFactor(ht) ht.used / ht.size
#define dictIsRehashing(dc) dc->rehashidx != DICTHT_DEFAULT_REHASHING
#define dicthtReset(ht) do {    \
    ht->used = 0;               \
    ht->size = 0;               \
    ht->sizemask = 0;           \
    ht->table = NULL;           \
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

static unsigned int normalizePower(unsigned int n) {
    unsigned int s = 1;
    while (s < n) s *= 2;
    return s;
}

static int initDictht(dictht* ht, unsigned int size) {
    ht->size = normalizePower(size);
    ht->sizemask = ht->size-1;
    ht->used = 0;
    ht->table = malloc(sizeof(dictEntry) * ht->size);
    return DICT_OK;
}

static dictEntry* dicthtInsert(dictht *ht, dictType *type, void *key) {
    unsigned int idx = dicthtEntryKeyIndex(ht, type, key);
    if (dicthtIsOccupied(ht, idx)) {
        dictEntry *next = dicthtGet(ht, idx);
        while (next != NULL) {
            if (type->keyCompare(next->k, key))
                return NULL;
            next = next->next;
        }
    }
    dictEntry *e = malloc(sizeof(*e));
    e->k = key;
    dicthtSet(ht, idx, e);
    dicthtIncrSize(ht);
    return e;
}

static void rehashLoops(dict *dc, int n) {
    // NOTE: redis is so particular that the empty loops is limited
    int empty_visits = n*10; /* Max number of empty buckets to visit. */

    dictht* h0 = &dc->ht[0];
    dictht* h1 = &dc->ht[1];

    dictEntry *next, *en;
    while (n-- && dc->ht[0].used != 0) {
        /* skip if table does not have anything at index */
        while (h0->table[dc->rehashidx] == NULL) {
            dc->rehashidx++;
            if (--empty_visits == 0) return;
        }

        en = dc->ht[0].table[dc->rehashidx];
        while (en != NULL) {
            next = en->next;
            dicthtSet(h1, dicthtEntryKeyIndex(h1, dc->type, en), en);
            dicthtIncrSize(h1);
            en = next;
            h0->used--;
        }
        h0->table[dc->rehashidx] = NULL;
        dc->rehashidx++;
    }

    if (h0->used == 0) {
        free(h0->table);
        dc->rehashidx = DICTHT_DEFAULT_REHASHING;
        dc->ht[0] = dc->ht[1];
        dicthtReset(h1);
    }
}

static void resizeDictht(dict *dc, unsigned int size) {
    initDictht(&dc->ht[1], size);
    dc->rehashidx = 0;
    return rehashLoops(dc, DICTHT_DEFAULT_REHASHING_LOOPS);
}

static void resizeIfNeeded(dict *dc) {
    if (dictIsRehashing(dc)) return;
    if (dicthtLoadFactor(dc->ht[0]) > DICTHT_LOADFACTOR_HI)
        return resizeDictht(dc, dc->ht[0].used*2);
}

dict *newDict(dictType *type) {
    dict *d = malloc(sizeof(*d));
    if (d == NULL) return NULL;

    d->type = type;
    d->rehashidx = DICTHT_DEFAULT_REHASHING;
    initDictht(&d->ht[0], DICTHT_DEFAULT_SIZE);
    return d;
}

int dictInsert(dict *dc, void *key, void *val) {
    if (dictIsRehashing(dc)) rehashLoops(dc, DICTHT_DEFAULT_REHASHING_LOOPS);

    resizeIfNeeded(dc);
    dictht *ht = dictIsRehashing(dc) ? &dc->ht[1] : &dc->ht[0];

    /* Note:
     * You have to admit that Redis is really particular about performance.
     * My original implementation is:
     *      dictEntry *entry = newDictEntry(key, val);  @1
     *      if (dictIsRehashing(dc)) rehashLoops(dc, DICTHT_DEFAULT_REHASHING_LOOPS);
     *      ...  // other code
     * But Redis's implementation is actually using only the key for insertion.
     * My original implementation is slower because not all key will be inserted
     * and hence @1 is wasting time in malloc unnecessary spaces.
     */
    dictEntry *entry = dicthtInsert(ht, dc->type, key);
    if (entry != NULL) entry->v.val = val;
    return DICT_OK;
}