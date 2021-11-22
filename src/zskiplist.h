//
// Created by lxm on 14/10/21.
//

#include "sds.h"

#define ZSL_MAX_LEVEL 32
#define ZSL_LEVEL_PROBABILITY 0.25
#define ZSL_OK 0
#define ZSL_ERROR 1

/* ZSETs use a specialized version of Skiplists */
typedef struct zskiplistNode {
    sds key;
    double score;
    struct zskiplistNode *backward;
    unsigned int height;
    /* This definition cannot be easily initialized with malloc
     * together with the outer struct.
     *  struct zskiplistLevel {
     *      struct zskiplistNode *forward;
     *      unsigned int span;
     *  }* level;
     */
    struct zskiplistLevel {
        struct zskiplistNode *forward;
        unsigned int span;
    } level[];
} zskiplistNode;

typedef struct zskiplist {
    zskiplistNode * head;
    zskiplistNode * tail;
    unsigned int length;
    unsigned int level;
} zskiplist;

zskiplist* zslCreate(void);
int zslInsert(zskiplist* zsl, sds key, double score);
//int zslGet(zskiplist* zsl, double score);