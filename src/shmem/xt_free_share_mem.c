#include "xt_headers.h"

/* 解除共享内存 */
int free_share_mem(int pShmProId, CONF_PRCSHRMEM_INFO *pShmPro) {
    int rc = 0;

    if (pShmPro != NULL) {
        rc = detach_share_mem(pShmProId, pShmPro);
        if (rc != MDIF_NORMAL) {
            ERRORLOG(__FUNCTION__);
            return MDIF_ERROR;
        }
    }
    return MDIF_NORMAL;
}
