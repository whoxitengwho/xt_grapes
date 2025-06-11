#include "xt_headers.h"

/* 外部变量 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;

void term_signal_grape(int iSignal);

int init_cluster_proc(void) {
    int rc = MDIF_ERROR;
    int i;
    pid_t Mypid;
    struct sigaction sa, old;

    INITIALIZE(i);
    INITIALIZE(Mypid);
    INITIALIZE(sa);
    INITIALIZE(old);
    printf("START IntializeManager ...\n");

    /* 创建共享内存 */
    rc = mdif_create_shared();
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 初期化进程表信息 */
    rc = init_app_conf();
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 取得当前父进程Id: 运行主Id */
    Mypid = getpid();

    /* 设置运行程序终了状态: OFF */
    gpShmPro->PmgrTermFlag = MDIF_FLG_OFF;

    /* 初期化所有线程数组表信息 */
    for (i = 0; i < THREADTBL_OCCURS; i++) {
        gpShmPro->Thread_Tbl[i].iThread_No = i;
        gpShmPro->Thread_Tbl[i].cStart_Status = MDIF_THREAD_STS_NOCREATE;
        gpShmPro->Thread_Tbl[i].cThread_Status = MDIF_THREAD_STS_NOTUSE;
    }
    for (i = 0; i < gpShmPro->MdIf_Info.iComProcess_Counter; i++) {
        gpShmPro->ComPrc_Tbl[i].iProcNo = i;
    }

    rc = get_sema(&gpShmPro->MdIf_Info.szThread_SemId);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    sa.sa_handler = term_signal_grape;
    sa.sa_flags = 0;
    sigaction(SIGCHLD, &sa, &old);

    rc = fork_grape();
    if (rc != MDIF_NORMAL) {
        rc = term_notify_grape();
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    return MDIF_NORMAL;
}