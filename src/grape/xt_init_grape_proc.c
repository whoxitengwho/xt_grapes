#include "xt_headers.h"

/* 共享内存指针 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存id */
extern int gShmProId;
/* 同步锁id */
extern int gSemId;
/* 进程单元指针 */
extern COMPRC_INFO *pMyPrcInfo;

/* 初期处理 */
int init_grape_proc() {
    int rc;
    int i;
    int j;
    int bExit;
    int iProcNo;
    /* 取得当前进程id */
    pid_t pid = getpid();
    INITIALIZE(rc);
    INITIALIZE(i);
    INITIALIZE(j);
    INITIALIZE(bExit);
    INITIALIZE(iProcNo);

    /* 取得共享内存 */
    rc = get_share_mem(&gShmProId, &gpShmPro);
    if (rc == MDIF_ERROR) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    bExit = 0;
    /* 循环确认当前进程有没有在进程数组表中 */
    for (j = 0; j < gpShmPro->MdIf_Info.iExecSts_Retry_Counter; j++) {
        for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
            if (gpShmPro->ComPrc_Tbl[i].pPid == pid) {
                /* 初始化工作进程的生存通知:ON */
                gpShmPro->ComPrc_Tbl[i].cSaveSts = MDIF_FLG_ON;
                /* 初始化工作进程使用中的线程个数:0 */
                gpShmPro->ComPrc_Tbl[i].iPrcThread_Cnt = 0;
                /* 从共享内存取得当前进程单元 */
                pMyPrcInfo = (COMPRC_INFO * ) & gpShmPro->ComPrc_Tbl[i];
                bExit = 1;
                /* 保存当前进程在共享内存的进程数组表中的index */
                iProcNo = i;
                break;
            }
        }
        /* 查找当前进程在进程数组表中的位置信息成功 */
        if (bExit != 0) {
            break;
        }
        /* 查找失败后睡眠:1000*1000微妙 */
        usleep(1000 * 1000);
    }

    /* 查找当前进程id是否存在进程数组表里: 不存在 */
    if (bExit == 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 选取共享内存中线程数组表中 一部分挂载到当前进程使用 */
    for (i = 0; i < THREADTBL_OCCURS; i++) {
        if ((i) % gpShmPro->MdIf_Info.iComProcess_Counter == pMyPrcInfo->iProcNo) {
            /* 初期化线程挂载到哪个进程id */
            gpShmPro->Thread_Tbl[i].iComProcess_Id = pMyPrcInfo->pPid;
            /* 初期化线程挂载到哪个进程号 */
            gpShmPro->Thread_Tbl[i].iComProcess_No = pMyPrcInfo->iProcNo;
            printf("子进程[%d]挂载线程[%d]成功\n", pMyPrcInfo->iProcNo, i);
        }
    }

    /* todo 自己业务的提前准备 */


    /* 通知父进程启动完成, 进程启动状态:ON */
    pMyPrcInfo->cProcSts = MDIF_FLG_ON;
    printf("工作子进程[%d]启动成功\n", pMyPrcInfo->iProcNo);
    /* 正常 */
    return MDIF_NORMAL;
}