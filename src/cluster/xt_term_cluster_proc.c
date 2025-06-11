#include "xt_headers.h"

/* 共享内存 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存Id */
extern int gShmProId;
/* 共享内存地址 */
extern int *gpMapAddr;

int term_cluster_proc(void) {
    int rc = MDIF_NORMAL;
    printf("Start term_cluster_proc ...\n");

    /* 解除共享内存 */
    rc = mdif_delete_shared();
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    printf("End  term_cluster_proc ...\n");
    /* 正常 */
    return MDIF_NORMAL;
}