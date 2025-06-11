#include "xt_headers.h"

/* 共享内存指针 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存id */
extern int gShmProId;
/* 同步锁id */
extern int gSemId;
/* 进程单元指针 */
extern COMPRC_INFO *pMyPrcInfo;

void AllThreadTermNotice(void);

void ending_work() {
    printf("START   ending_work ...\n");
    /* 通知当前进程的所有线程: 终止任务 */
    if (gpShmPro != NULL) {
        AllThreadTermNotice();
        if (pMyPrcInfo != NULL) {
            pMyPrcInfo->cSaveSts = MDIF_FLG_OFF;
            printf("当前进程[%d]生存状态已经设置OFF\n", pMyPrcInfo->pPid);
        }
    }

    /* 结束共享内存指针 */
    free_share_mem(gShmProId, gpShmPro);
    printf("END   ending_work ...\n");

    /* 正常 */
    return;
}

void AllThreadTermNotice(void) {
    int i, j;
    pid_t pid = getpid();
    INITIALIZE(i);
    INITIALIZE(j);

    printf("Start AllThreadTermNotice ...\n");
    for (i = 0; i < THREADTBL_OCCURS; i++) {
        /* 查找属于当前进程的线程 */
        if (gpShmPro->Thread_Tbl[i].iComProcess_Id == pid) {
            /* 线程创建了正在使用: 通知结束任务 */
            if (gpShmPro->Thread_Tbl[i].cStart_Status == MDIF_THREAD_STS_CREATED) {
                /* 设置线程终了状态: ON */
                gpShmPro->Thread_Tbl[i].cThreadTermFlag = MDIF_FLG_ON;
            }
        }
    }

    /* 循环等待所有线程终了 */
    for (j = 0; j < MDIF_CONF_THREADTERM_TRYCOUNT; j++) {
        if (pMyPrcInfo->iPrcThread_Cnt == 0) {
            printf("当前进程[%d]全部线程已经终了\n", pMyPrcInfo->pPid);
            return;
        }
        sleep(2);
    }

    ERRORLOG(__FUNCTION__);
    printf("终了当前进程[%d]全部线程时发生错误\n", pMyPrcInfo->pPid);

    /* 异常 */
    return;
}