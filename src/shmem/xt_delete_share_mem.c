#include "xt_headers.h"

int delete_share_mem(int iShmId, void *pShmTop) {
    int rc;
    rc = 0;
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
    rc = shmctl(iShmId, IPC_RMID, (struct shmid_ds *) 0);
    if (rc < 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    return MDIF_NORMAL;
}
