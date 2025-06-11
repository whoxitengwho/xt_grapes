#include "xt_headers.h"

/*外部变量 */
extern CONF_PRCSHRMEM_INFO *gpShmPro;
extern int gShmProId;
extern int *gpMapAddr;

int mdif_delete_shared(void) {
    int rc = 0;
    int err_cnt = 0;

    printf("Start FreeSharedArea ...");
    if (gpShmPro != NULL && gpShmPro->MdIf_Info.szThread_SemId != 0) {
        rc = mdifCmnSemaDelete(gpShmPro->MdIf_Info.szThread_SemId);
        if (rc != MDIF_NORMAL) {
            ERRORLOG(__FUNCTION__);
            err_cnt++;
        }
        printf("szThread_SemId: [%d]", gpShmPro->MdIf_Info.szThread_SemId);
    }


    if (gpShmPro != NULL) {
        rc = delete_share_mem(gShmProId, gpShmPro);
        if (rc != MDIF_NORMAL) {
            ERRORLOG(__FUNCTION__);
            err_cnt++;
        }
    }

    if (gpMapAddr != NULL) {
        rc = munmap(gpMapAddr, sizeof(int));
        if (rc == -1) {
            ERRORLOG(__FUNCTION__);
            err_cnt++;
        }
    }

    rc = unlink(MDIF_CONF_PRCSHRMEM_PATH);
    if (rc != 0) {
        ERRORLOG(__FUNCTION__);
        err_cnt++;
    }

    if (err_cnt != 0) {
        printf("End   FreeSharedArea ... error");
        return MDIF_ERROR;

    } else {
        printf("End   FreeSharedArea ... success");
        return MDIF_NORMAL;
    }
}