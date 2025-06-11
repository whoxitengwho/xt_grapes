#include "xt_headers.h"

/* 外部变量 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;

int listen_grape(void) {
    int iExit = 1;
    int iRet = 0;
    int rc = MDIF_NORMAL;
    printf("START listen_grape ...\n");

    /* 主进程管理子进程: 循环查看终了标记:OFF */
    while (iExit) {
        if (gpShmPro->PmgrTermFlag == MDIF_FLG_ON) {
            printf("终止所有工作子进程 ...\n");
            rc = term_notify_grape();
            if (rc == MDIF_NORMAL) {
                iRet = MDIF_NORMAL;
                break;
            } else {
                printf("工作子进程终了失败 ...\n");
                gpShmPro->PmgrTermFlag = MDIF_FLG_OFF;
            }
        }
        usleep(gpShmPro->MdIf_Info.iMsgComWaitTime);
    }
    printf("END   listen_grape ...\n");

    /* 正常 */
    return iRet;
}


