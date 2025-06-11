#include "xt_headers.h"

extern CONF_PRCSHRMEM_INFO *gpShmPro;

/* 初期化进程表信息 */
int init_app_conf(void) {
    int i = 0;
    char cInts[18];
    int rc = 0;
    memset(&cInts, '\0', sizeof(cInts));
    /* 进程个数 */
    rc = get_property("../conf/xt_grape.conf", \
        "process_count", "PROCESS_COUNT", cInts);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    gpShmPro->MdIf_Info.iComProcess_Counter = atoi(cInts);
    printf("PROCESS_COUNT:%d\n", atoi(cInts));

    /* 主监听工作子进程状态间隔时间:微妙 */
    memset(&cInts, '\0', sizeof(cInts));
    rc = get_property("../conf/xt_grape.conf", \
        "intervalOfAppLstnPrs", "INTERVAL_TIME", cInts);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    gpShmPro->MdIf_Info.iMsgComWaitTime = atoi(cInts);
    printf("INTERVAL_TIME:%d\n", atoi(cInts));

    /* 主确认所有工作子进程是否已经启动次数 */
    memset(&cInts, '\0', sizeof(cInts));
    rc = get_property("../conf/xt_grape.conf", \
        "countOfAppCnfmPrs", "APP_CONFIRM_PRS_RETRY_COUNTER", cInts);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    gpShmPro->MdIf_Info.iExecSts_Retry_Counter = atoi(cInts);
    printf("APP_CONFIRM_PRS_RETRY_COUNTER:%d\n", atoi(cInts));

    /* 确认线程是否启动次数 */
    memset(&cInts, '\0', sizeof(cInts));
    rc = get_property("../conf/xt_grape.conf", \
        "countOfPrsCnfmThread", "PRS_CONFIRM_THREAD_RETRY_COUNTER", cInts);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    gpShmPro->MdIf_Info.iExecThread_Retry_Counter = atoi(cInts);
    printf("PRS_CONFIRM_THREAD_RETRY_COUNTER:%d\n", atoi(cInts));

    return MDIF_NORMAL;
}