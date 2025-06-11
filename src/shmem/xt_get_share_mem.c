#include "xt_headers.h"

/* 得到共享内存 */
int get_share_mem(int *pShmProId, CONF_PRCSHRMEM_INFO **pShmPro) {
    int rc = 0;
    char *pShmPath = NULL;
    void *pVShmPro = NULL;
    int fd = 0;
    int *pMapAddr = NULL;
    *pShmPro = NULL;

    /* 共享内存还没有取过: 共享内存id == 0 */
    if (*pShmProId == 0) {
        /* 取得共享内存全路径 + 文件名 */
        pShmPath = MDIF_CONF_PRCSHRMEM_PATH;
        if (pShmPath == NULL) {
            ERRORLOG(__FUNCTION__);
            return MDIF_ERROR;
        }
        /* 打开共享内存文件:open */
        fd = open(pShmPath, O_RDONLY);
        if (fd == -1) {
            ERRORLOG(__FUNCTION__);
            return MDIF_ERROR;
        }
        /* 将共享内存文件映射进内存 */
        pMapAddr = (int *) mmap(NULL, sizeof(int), PROT_READ, MAP_SHARED, fd, 0);
        if (pMapAddr == MAP_FAILED) {
            ERRORLOG(__FUNCTION__);
            return MDIF_ERROR;
        }

        *pShmProId = *pMapAddr;
        /* 解除共享内存的对象映射 */
        rc = munmap(pMapAddr, sizeof(int));
        if (rc == -1) {
            ERRORLOG(__FUNCTION__);
            return MDIF_ERROR;
        }

        /* 关闭文件读取流 */
        rc = close(fd);
        if (rc == -1) {
            ERRORLOG(__FUNCTION__);
            return MDIF_ERROR;
        }
    }
    /* 把共享内存区对象映射到调用进程的地址空间 */
    rc = attach_share_mem(*pShmProId, &pVShmPro);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 取得共享内存指针 */
    *pShmPro = (CONF_PRCSHRMEM_INFO *) pVShmPro;
    /* 正常 */
    return MDIF_NORMAL;
}
