#include "xt_headers.h"

/* 共享内存指针 */
CONF_PRCSHRMEM_INFO *gpShmPro = NULL;
/* 共享内存id */
int gShmProId = 0;
/* 同步锁id */
int gSemId = 0;
/* 进程单元 */
COMPRC_INFO *pMyPrcInfo = NULL;

/* 同步锁:更新线程数 */
pthread_mutex_t global_threadcnt_mutex = PTHREAD_MUTEX_INITIALIZER;
/* 同步锁:更新线程状态 */
pthread_mutex_t global_threadsts_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
    int rc;
    INITIALIZE(rc);

    /* 初期处理 */
    rc = init_grape_proc();
    if (rc == MDIF_NORMAL) {
        /* 主处理 */
        rc = doing_work();
    }

    /* 终了处理 */
    ending_work();

    /* 正常处理 */
    return MDIF_NORMAL;
}