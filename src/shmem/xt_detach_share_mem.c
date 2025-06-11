#include "xt_headers.h"

int detach_share_mem(int iShmId, void *pShmTop) {
    int rc;
    INITIALIZE(rc);

    if (iShmId == 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    if (pShmTop == NULL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    rc = shmdt(pShmTop);
    if (rc < 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    pShmTop = NULL;

    return MDIF_NORMAL;
}
