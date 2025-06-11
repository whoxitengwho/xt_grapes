#include "xt_headers.h"

/* 共享内存指针 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存id */
extern int gShmProId;
/* 同步锁id */
extern int gSemId;
/* 进程单元指针 */
extern COMPRC_INFO *pMyPrcInfo;

int ExecThreadProc(THREAD_TBL *pstThreadTbl);

void working_thread(THREAD_TBL *pstThread);

int create_thread() {
    int i;
    int j;
    THREAD_TBL *pstThreadTbl = NULL;
    int bExit;
    int rc;
    INITIALIZE(i);
    INITIALIZE(j);
    INITIALIZE(bExit);
    INITIALIZE(rc);

    /* 锁共享内存 */
    rc = lock_sema(gpShmPro->MdIf_Info.szThread_SemId);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 寻找一个可以使用的线程 */
    for (i = 0; i < THREADTBL_OCCURS; i++) {

        if ((gpShmPro->Thread_Tbl[i].iComProcess_Id == pMyPrcInfo->pPid) &&
            (gpShmPro->Thread_Tbl[i].cStart_Status == MDIF_THREAD_STS_NOCREATE)) {
            pstThreadTbl = (THREAD_TBL * ) & gpShmPro->Thread_Tbl[i];
            printf("找到可使用线程序列号[%d]，进程号[%d]\n", i, gpShmPro->Thread_Tbl[i].iComProcess_No);
            break;
        }
    }

    /* 查找可使用的线程: 无 */
    if (pstThreadTbl == NULL) {
        // todo 如果没有找到，说明压力太大，return MDIF_ERROR
        // 需要适当调整初始化进程和线程数，前提是硬件扛得住
        printf("没有找到空余线程,放弃新请求的任务\n");
        goto end;
    }

    rc = ExecThreadProc(pstThreadTbl);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 循环确认线程是否启动 */
    bExit = 0;
    for (j = 0; j < gpShmPro->MdIf_Info.iExecThread_Retry_Counter; j++) {
        usleep(30 * 1000);
        if (pstThreadTbl->cStart_Status == MDIF_THREAD_STS_CREATED) {
            bExit = 1;
            break;
        }
    }
    /* 查找可以使用的线程: 失败 */
    if (bExit == 0) {
        pstThreadTbl->cThread_Status = MDIF_THREAD_STS_INTROUBLE;
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 设置线程状态: 已经被使用 */
    pstThreadTbl->cThread_Status = MDIF_THREAD_STS_USED;

end:
    /* 释放锁共享内存 */
    rc = unlock_sema(gpShmPro->MdIf_Info.szThread_SemId);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 正常 */
    return MDIF_NORMAL;
}


int ExecThreadProc(THREAD_TBL *pstThreadTbl) {
    int rc;
    pthread_t NewThread;
    pid_t pid = getpid();

    INITIALIZE(rc);
    INITIALIZE(NewThread);

    /* 设置当前线程状态: 使用中 */
    pstThreadTbl->cStart_Status = MDIF_THREAD_STS_CREATING;
    pstThreadTbl->iComProcess_Id = pid;
    /* 设置线程终了状态: OFF */
    pstThreadTbl->cThreadTermFlag = MDIF_FLG_OFF;
    /* 设置当前线程id */
    pstThreadTbl->thread_Id = NewThread;

    /* 线程种类: 消息发送 */
    rc = pthread_create(&NewThread,
                        0,
                        (void *(*)(void *)) working_thread,
                        (void *) pstThreadTbl);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 解除NewThread */
    pthread_detach(NewThread);

    /* 正常 */
    return MDIF_NORMAL;
}