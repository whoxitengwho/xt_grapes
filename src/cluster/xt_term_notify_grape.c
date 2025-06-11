#include "xt_headers.h"


extern CONF_PRCSHRMEM_INFO *gpShmPro;

int term_notify_grape(void) {
    int iRet;
    int i, j;
    struct sigaction sa, old;

    INITIALIZE(iRet);
    INITIALIZE(i);
    INITIALIZE(j);
    INITIALIZE(sa);
    INITIALIZE(old);
    printf("START ProcessTerminateNotice ...\n");
    /* 信号工作: 主监听工作的子进程关闭:SIGCHLD
     */
    sa.sa_handler = SIG_IGN;
    sigaction(SIGCHLD, &sa, &old);

    /* 设置所有工作子进程终了标记: ON */
    for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
        if (gpShmPro->ComPrc_Tbl[i].cSaveSts != MDIF_FLG_OFF) {
            gpShmPro->ComPrc_Tbl[i].cExitSts = MDIF_FLG_ON;
        }
    }

    sleep(5);
    /* 确认所有工作子进程是否已经终止 */
    for (j = 0; j < MDIF_CONF_PROCESSTERM_TRYCOUNT; j++) {
        printf("开始第[%d]次确认所有工作子进程是否终止\n", j);
        iRet = MDIF_NORMAL;
        for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
            if (gpShmPro->ComPrc_Tbl[i].cSaveSts == MDIF_FLG_ON) {
                iRet = MDIF_ERROR;
                printf("第[%d]个工作子进程未终止\n", i);
                break;
            }
        }
        /* 所有工作子进程已经终止 */
        if (iRet == MDIF_NORMAL) {
            printf("所有工作子进程已经终止\n");
            return MDIF_NORMAL;
        }
        printf("结束第[%d]次确认所有工作子进程是否终止\n", j);
        sleep(1);
    }

    printf("\n确认子进程是否终止次数=[%d]\n", j);
    for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
        if (gpShmPro->ComPrc_Tbl[i].cSaveSts != MDIF_FLG_OFF) {
            printf("出错进程 : %d\n", gpShmPro->ComPrc_Tbl[i].pPid);
        }
    }

    /* 异常 */
    return iRet;
}
