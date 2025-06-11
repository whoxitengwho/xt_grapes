#include "xt_headers.h"

/* 共享内存指针 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存id */
extern int gShmProId;
/* 同步锁id */
extern int gSemId;
/* 进程单元指针 */
extern COMPRC_INFO *pMyPrcInfo;

void free_thread(THREAD_TBL *pstThread) {
    int rc;
    INITIALIZE(rc);
    /* 锁共享内存 */
    rc = lock_sema(gpShmPro->MdIf_Info.szThread_SemId);
    if (rc == MDIF_ERROR) {
        ERRORLOG(__FUNCTION__);
        return;
    }

    memset(pstThread->cApp_Id, '0', sizeof(pstThread->cApp_Id));
    pstThread->thread_Id = 0;
    pstThread->cStart_Status = MDIF_THREAD_STS_NOCREATE;
    pstThread->cThread_Status = MDIF_THREAD_STS_NOTUSE;
    pstThread->cThreadTermFlag = MDIF_FLG_OFF;

    /* 解除锁共享内存 */
    rc = unlock_sema(gpShmPro->MdIf_Info.szThread_SemId);
    if (rc == MDIF_ERROR) {
        ERRORLOG(__FUNCTION__);
    }

    /* 正常 */
    return;
}
