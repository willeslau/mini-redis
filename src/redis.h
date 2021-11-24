//
//#include "dict.h"
//#include "sds.h"
//
//struct evictionPoolEntry {
//    unsigned long long idle;    /* Object idle time. */
//    sds key;                    /* Key name. */
//};
//
//typedef struct redisDb {
//    dict *dict;                 /* The keyspace for this DB */
//    dict *expires;              /* Timeout of keys with a timeout set */
//    dict *blocking_keys;        /* Keys with clients waiting for data (BLPOP) */
//    dict *ready_keys;           /* Blocked keys that received a PUSH */
//    dict *watched_keys;         /* WATCHED keys for MULTI/EXEC CAS */
//    struct evictionPoolEntry *eviction_pool;    /* Eviction pool of keys */
//    int id;                     /* Database ID */
//    long long avg_ttl;          /* Average TTL, just for stats */
//} redisDb;
//
///* The actual Redis Object */
//#define REDIS_LRU_BITS 24
//#define REDIS_LRU_CLOCK_MAX ((1<<REDIS_LRU_BITS)-1) /* Max value of obj->lru */
//#define REDIS_LRU_CLOCK_RESOLUTION 1000 /* LRU clock resolution in ms */
//typedef struct redisObject {
//    unsigned type:4;
//    unsigned encoding:4;
//    unsigned lru:REDIS_LRU_BITS; /* lru time (relative to server.lruclock) */
//    int refcount;
//    void *ptr;
//} robj;
//
//robj *lookupKeyWrite(redisDb *db, robj *key);
//robj *lookupKey(redisDb *db, robj *key);
//void dbAdd(redisDb *db, robj *key, robj *val);
//void dbOverwrite(redisDb *db, robj *key, robj *val);
//void setKey(redisDb *db, robj *key, robj *val);