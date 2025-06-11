#include "xt_headers.h"

/* 共享内存指针 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存id */
extern int gShmProId;
/* 同步锁id */
extern int gSemId;
/* 进程单元指针 */
extern COMPRC_INFO *pMyPrcInfo;


int doing_work() {
    int iRet = MDIF_NORMAL;
    int rc;
    int bExit = 1;

    INITIALIZE(rc);

    while (bExit) {
        if (pMyPrcInfo->cExitSts == MDIF_FLG_ON) {
            printf("工作进程[%d]终止中 ...\n", getpid());
            break;
        }

        /* todo 自己业务
         * 在什么场景下需要开启执行任务，
         * 例如：建立远程连接
         * */

        rc = create_thread();
        if (rc != MDIF_NORMAL) {
            iRet = MDIF_ERROR;
            ERRORLOG(__FUNCTION__);
            break;
        }

    }

    /* 正常或异常 */
    return iRet;
}