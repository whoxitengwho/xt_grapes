#include "xt_headers.h"

int create_share_mem(int iShmSize, int *pShmId, void **pShmTop) {
    int shmflg = 0;

    shmflg = IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

    *pShmId = shmget(IPC_PRIVATE, iShmSize, shmflg);
    if (*pShmId == -1) {
        ERRORLOG(__FUNCTION__);
        *pShmId = 0;
        return MDIF_ERROR;
    }

    void *addr = shmat(*pShmId, NULL, 0);
    if (addr == (void *) -1) {
        ERRORLOG(__FUNCTION__);
        *pShmTop = NULL;
        return MDIF_ERROR;
    }
    memset(addr, 0x00, iShmSize);
    *pShmTop = addr;

    return MDIF_NORMAL;
}

