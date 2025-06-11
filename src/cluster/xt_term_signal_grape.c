#include "xt_headers.h"

extern CONF_PRCSHRMEM_INFO *gpShmPro;

/* 主进程创建的工作进程是否正常关闭
 * 工作子进程关闭时, 此函数就被调用
 */
void term_signal_grape(int iSignal) {
    int iPID;
    int i;
    int j;
    int stat_Ioc;
    int rc;
    INITIALIZE(iPID);
    INITIALIZE(i);
    INITIALIZE(j);
    INITIALIZE(stat_Ioc);
    INITIALIZE(rc);

    for (;;) {
        /* 取得关闭的子进程id */
        iPID = wait3(&stat_Ioc, WNOHANG, NULL);
        if (iPID <= 0) {
            break;
        }

        /* 检测被关闭的子进程是否是正常关闭
         * 主终止开关: 不关闭: 此一个子进程属于异常关闭
        */
        for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
            if (gpShmPro->ComPrc_Tbl[i].pPid == iPID) {
                /* 锁住共享内存 */
                rc = lock_sema(gpShmPro->MdIf_Info.szThread_SemId);
                if (rc != MDIF_NORMAL) {
                    ERRORLOG(__FUNCTION__);
                }

                gpShmPro->ComPrc_Tbl[i].cSaveSts = MDIF_FLG_OFF;

                /* 主终止开关：不关闭:
                 * 设置被关闭的进程的所有线程状态: 障碍(trouble)
                */
                if (gpShmPro->PmgrTermFlag != MDIF_FLG_ON) {
                    for (j = 0; j < THREADTBL_OCCURS; j++) {
                        if (gpShmPro->ComPrc_Tbl[i].pPid == gpShmPro->Thread_Tbl[j].iComProcess_Id) {
                            gpShmPro->Thread_Tbl[j].cThread_Status = MDIF_THREAD_STS_INTROUBLE;
                        }
                    }
                }

                /* 解锁共享内存 */
                rc = unlock_sema(gpShmPro->MdIf_Info.szThread_SemId);
                if (rc != MDIF_NORMAL) {
                    ERRORLOG(__FUNCTION__);
                }
                ERRORLOG(__FUNCTION__);
                /* 子进程有错 */
                printf("子进程iPID:[%d]有异常已经关闭\n", iPID);
                break;
            }
        }
    }

    /* 正常或异常 */
    return;
}
