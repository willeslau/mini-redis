/*
 * This is a study of the Redis SDS data structure and its related functions.
 */

#include <malloc.h>
#include <string.h>
#include "sds.h"

unsigned int sdslen(sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

sds newsdslen(const char* init[], size_t initlen) {
    struct sdshdr* sh;

    if (initlen) {
        sh = malloc(sizeof(struct sdshdr) + initlen + 1);
    } else {
        sh = malloc(sizeof(struct sdshdr));
    }

    if (sh == NULL) { return NULL; }

    sh->len = initlen;
    sh->free = 0;

    if (init && initlen)
        memcpy(sh->buffer, init, initlen);

    return sh->buffer;
}

/* Free an sds string. No operation is performed if 's' is NULL. */
void sdsfree(sds s) {
    if (s == NULL) return;
    free(s-sizeof(struct sdshdr));
}