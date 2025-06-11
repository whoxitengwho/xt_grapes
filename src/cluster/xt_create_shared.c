#include "xt_headers.h"

/* 共享内存 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
/* 共享内存Id */
extern int gShmProId;
/* 共享内存地址 */
extern int *gpMapAddr;

int mdif_create_shared(void) {
    int rc;               /* return值 */
    void *pShrmem;          /* 共享内存块首地址 */
    char *pShmPath = NULL;  /* 共享内存映射的文件 */
    int fd = 0;            /* 文件描述符 */

    INITIALIZE(rc);
    INITIALIZE(pShrmem);

    rc = create_share_mem(sizeof(CONF_PRCSHRMEM_INFO), &gShmProId, &pShrmem);
    if (rc != MDIF_NORMAL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    gpShmPro = pShrmem;

    pShmPath = MDIF_CONF_PRCSHRMEM_PATH;
    if (pShmPath == NULL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    fd = open(pShmPath, O_RDWR | O_CREAT | O_TRUNC | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, 777);
    if (fd == -1) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    rc = write(fd, &gShmProId, sizeof(int));
    if (rc < 0) {
        ERRORLOG(__FUNCTION__);
        close(fd);
        return MDIF_ERROR;
    }
    /* mapping */
    gpMapAddr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (gpMapAddr == MAP_FAILED) {
        gpMapAddr = NULL;
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    /* 关闭文件 */
    rc = close(fd);
    if (rc == -1) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    /* 正常 */
    return MDIF_NORMAL;
}
