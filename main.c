//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include "src/ziplist.h"
//
//void mine() {
//    #include "src/zskiplist.h"
//    zskiplist* zsl = zslCreate();
//    clock_t start_time = clock();
//    for (int i = 0; i < 1000; i++) {
//        char str[4];
//        sprintf(str, "%d", i);
//        sds s = newsdslen((const char **) str, 3);
//
//        zslInsert(zsl, s, (double)i);
//    }
//
//    clock_t elapsed_time = clock() - start_time;
//    printf("Done in %ld us\n", elapsed_time);
//}
//
//void redis() {
//    #include "src/redis_zskip.h"
//
//    zskiplist* zsl = rediszslCreate();
//
//    clock_t start_time = clock();
//    for (int i = 0; i < 1000; i++) {
//        char str[4];
//        sprintf(str, "%d", i);
//        sds s = newsdslen((const char **) str, 3);
//
//        rediszslInsert(zsl, (double) i, s);
//    }
//
//    clock_t elapsed_time = clock() - start_time;
//    printf("Done in %ld us\n", elapsed_time);
//}
//
//int main() {
//    srand(time(NULL));
//    mine();
//    redis();
//    printf("done");
//    return 0;
//}
