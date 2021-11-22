typedef char* sds;

struct sdshdr {
    unsigned int len;
    unsigned int free;
    char buffer[];
};

unsigned int sdslen(sds s);
sds newsdslen(const char* init[], size_t initlen);
void sdsfree(sds s);