#include "xt_headers.h"

/* 共享内存 */
CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存Id */
int gShmProId;
/* 共享内存地址 */
int *gpMapAddr;

int main(int argc, char *argv[]) {
    int rc;
    int rc1;
    int rc2;
    int rc3;
    INITIALIZE(rc);
    INITIALIZE(rc1);
    INITIALIZE(rc2);
    INITIALIZE(rc3);
    rc = MDIF_ERROR;
    rc1 = MDIF_ERROR;
    rc2 = MDIF_ERROR;
    rc3 = MDIF_ERROR;

    /* 初期处理 */
    rc1 = init_cluster_proc();
    if (rc != MDIF_NORMAL) {
        /* 主处理 */
        rc2 = listen_grape();
    }

    /* 终了处理 */
    rc3 = term_cluster_proc();
    if ((rc1 == MDIF_NORMAL) && (rc2 == MDIF_NORMAL) && (rc3 == MDIF_NORMAL)) {
        rc = MDIF_NORMAL;
    } else {
        ERRORLOG(__FUNCTION__);
        rc = MDIF_ERROR;
    }

    /* 正常或异常 */
    return rc;
}