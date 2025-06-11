#include "xt_headers.h"

/* 把共享内存区对象映射到调用进程的地址空间 */
int attach_share_mem(int iShmId, void **pShmTop) {
    *pShmTop = (void *) shmat(iShmId, 0, 0);
    if (*pShmTop == (void *) -1) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    return MDIF_NORMAL;
}