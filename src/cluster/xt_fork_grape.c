#include "xt_headers.h"

/* 共享内存 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存Id */
extern int gShmProId;
/* 共享内存地址 */
extern int *gpMapAddr;


int fork_grape(void) {
    int rc = MDIF_NORMAL;

    rc = execGrapeProcess();
    if (rc != MDIF_NORMAL) {
        return MDIF_ERROR;;
    }

    /* 正常 */
    return MDIF_NORMAL;
}


/* 主循环创建工作子进程 */
int execGrapeProcess(void) {
    int i, j;
    int iStartFlg;
    pid_t iPid = 0;

    INITIALIZE(i);
    INITIALIZE(j);
    INITIALIZE(iStartFlg);

    for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
        gpShmPro->ComPrc_Tbl[i].cProcSts = MDIF_FLG_OFF;
        gpShmPro->ComPrc_Tbl[i].cSaveSts = MDIF_FLG_OFF;
        gpShmPro->ComPrc_Tbl[i].cExitSts = MDIF_FLG_OFF;
        iPid = fork();
        if (iPid < 0) {
            ERRORLOG(__FUNCTION__);
            return MDIF_ERROR;
        }
        if (iPid == 0) {
            execl("grape", "grape", NULL);
            exit(1);
        } else if (iPid > 0) {
            gpShmPro->ComPrc_Tbl[i].pPid = iPid;
            printf("工作子进程（%d） PID=[%d]\n", i + 1, iPid);
        }
    }

    /* 确认所有工作子进程是否启动完成 */
    for (j = 0; j < gpShmPro->MdIf_Info.iExecSts_Retry_Counter; j++) {
        iStartFlg = 0;
        for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
            if (gpShmPro->ComPrc_Tbl[i].cProcSts != MDIF_FLG_ON) {
                printf("第[%d]次检测所有工作子进程是否启动成功\n", j + 1);
                iStartFlg = 1;
                sleep(1);
                break;
            }
        }
    }
    if (iStartFlg == 1) {
        printf("第（%d）个工作子进程启动异常 PID=[%d]\n", i, gpShmPro->ComPrc_Tbl[i].pPid);
        return MDIF_ERROR;
    }
    printf("\n所有工作子进程启动成功\n");

    /* 正常 */
    return MDIF_NORMAL;
}

