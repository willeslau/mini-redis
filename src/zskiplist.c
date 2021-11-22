//
// Created by lxm on 14/10/21.
//

#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include "zskiplist.h"

#define random() ((double)rand() / (double)RAND_MAX)

unsigned int randomLevel(double p) {
    unsigned int level = 1;
    while (random() <= p && level < ZSL_MAX_LEVEL) level++;
    return level;
}

/*
 * Insert node into the chain of nodes.
 */
static void _insert(zskiplistNode** pNode, zskiplistNode* toInsert, unsigned int levelNum) {
    if (*pNode == NULL) {
        *pNode = toInsert;
        return;
    }

    struct zskiplistLevel* level = (*pNode)->level + levelNum;
    zskiplistNode* next = level->forward;

    if (next == NULL || ((*pNode)->score < toInsert->score && toInsert->score <= next->score)) {
        level->forward = toInsert;
        toInsert->backward = *pNode;
        return;
    }

    _insert(&next, toInsert, levelNum);
}

static zskiplistNode* zslNodeCreate(sds key, double score, unsigned int height) {
    zskiplistNode* zslNode;

    zslNode = malloc(sizeof(*zslNode) + sizeof(struct zskiplistLevel) * height);
    if (zslNode == NULL) return NULL;

    zslNode->key = key;
    zslNode->score = score;
//    zslNode->height = height;

    /* This leads to around 25% increase in performance. */
//    zslNode->level = (zskiplistLevel*) malloc(sizeof(zskiplistLevel) * height);
//    if (zslNode->level == NULL) {
//        free(zslNode);
//        return NULL;
//    }

    return zslNode;
}

zskiplist* zslCreate(void) {
    zskiplist* zsl;

    zsl = malloc(sizeof(*zsl));
    if (zsl == NULL) { return NULL; }

    zsl->length = 0;
    zsl->level = 1;
    zsl->head = zslNodeCreate(NULL, 0.0, ZSL_MAX_LEVEL);
    for (int j = 0; j < ZSL_MAX_LEVEL; j++) {
        zsl->head->level[j].forward = NULL;
        zsl->head->level[j].span = 0;
    }
    zsl->head->backward = NULL;
    zsl->tail = NULL;
    return zsl;
}

int zslInsert(zskiplist* zsl, sds key, double score) {
    zskiplistNode *update[ZSL_MAX_LEVEL];

    /* Note that x is shifted out of the loop to increase speed */
    zskiplistNode* x = zsl->head;

    /* Note that if we generate the level here:
     *      unsigned int level = randomLevel(ZSL_LEVEL_PROBABILITY);
     *      for (int i = level-1; i >= 0; i--)
     *      ...
     * The overall speed is greatly reduced. This is very interesting!
     * This is actually some very deep understanding of the underlying algorithm!
     * zsl->level is tracking the max level seen. The testing data generated
     * is sorted in descending order, which means a new item is always inserted
     * to the end. If we are using the max level so far, then we can guarantee we
     * are going the fastest path to the end. This greatly reduced the number of
     * searches. Very good!
     */
    for (int i = zsl->level-1; i >= 0; i--) {
        /* note that this recursive code:
         *      zskiplistLevel* level = zsl->head->level + i;
         *      _insert(&level->forward, node, i)
         * is slightly slower than the iterative code.
         * It can be made into tail recursive, but it looks like subject to the compiler
         * if optimizations will ever be made.
         */
        while (x->level[i].forward && x->level[i].forward->score < score) {
            x = x->level[i].forward;
        }
        update[i] = x;
    }

    /* Note that in the redis implementation, there is a lazy initialization of the levels.
     * This is why redis is good, it is trying to do things lazily and also reduce space
     * as much as possible! Not just this, refer to the above section.
     */
    unsigned int level = randomLevel(ZSL_LEVEL_PROBABILITY);
    if (level > zsl->level) {
        for (int i = zsl->level; i < level; i++) {
            update[i] = zsl->head;
            update[i]->level[i].span = zsl->length;
        }
        zsl->level = level;
    }
    x = zslNodeCreate(key, score, level);
    for (int i = 0; i < level; i++) {
        x->level[i].forward = update[i]->level[i].forward;
        update[i]->level[i].forward = x;
    }

    x->backward = (update[0] == zsl->head) ? NULL : update[0];
    if (x->level[0].forward)
        x->level[0].forward->backward = x;
    else
        zsl->tail = x;
    return ZSL_OK;
}