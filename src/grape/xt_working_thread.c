#include "xt_headers.h"

/* 共享内存指针 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存id */
extern int gShmProId;
/* 同步锁id */
extern int gSemId;
/* 进程单元指针 */
extern COMPRC_INFO *pMyPrcInfo;
/* 当前线程 */
extern THREAD_TBL *pstThreadTbl;

/* 同步锁:更新线程数    */
extern pthread_mutex_t global_threadcnt_mutex;
/* 同步锁:更新线程状态    */
extern pthread_mutex_t global_threadsts_mutex;


void working_thread(THREAD_TBL *pstThread) {
    int iRet = MDIF_ERROR;
    pthread_t tid = pthread_self();

    pthread_mutex_lock(&global_threadcnt_mutex);
    pMyPrcInfo->iPrcThread_Cnt = pMyPrcInfo->iPrcThread_Cnt + 1;
    pthread_mutex_unlock(&global_threadcnt_mutex);

    /* 通知进程这个线程启动状态:启动 */
    pstThread->cStart_Status = MDIF_THREAD_STS_CREATED;
    /* 设置当前线程id */
    pstThread->thread_Id = tid;
    printf("第[%d]进程，总共分配了[%d]个线程处理任务\n", pMyPrcInfo->iProcNo + 1, pMyPrcInfo->iPrcThread_Cnt);

    /* todo 自己业务
       if(iRet != MDIF_NORMAL){
             ERRORLOG(__FUNCTION__);
       }
   */
    usleep(30000 * 1000); //todo 模拟工作量极限30秒才能处理完
    /* 初期化线程 */
    free_thread(pstThread);
    pthread_mutex_lock(&global_threadcnt_mutex);
    pMyPrcInfo->iPrcThread_Cnt = pMyPrcInfo->iPrcThread_Cnt - 1;
    pthread_mutex_unlock(&global_threadcnt_mutex);

    /* 正常 */
    pthread_exit(NULL);
}


