//#include "redis.h"
//
//void incrRefCount(robj *o) {
//    o->refcount++;
//}
//
//void setKey(redisDb *db, robj *key, robj *val) {
//    if (lookupKeyWrite(db, key)) dbOverwrite(db ,key, val);
//    else dbAdd(db, key, val);
//
//    incrRefCount(val);
////    removeExpire(db,key);
////    signalModifiedKey(db,key);
//}
