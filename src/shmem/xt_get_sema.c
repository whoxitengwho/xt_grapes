#include "xt_headers.h"


int get_sema(int *iSemId) {
    int rc;
    int iSemFlg;
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } c_arg;

    INITIALIZE(rc);
    INITIALIZE(iSemFlg);
    INITIALIZE(c_arg);

    iSemFlg = 0666 | IPC_CREAT | SEM_UNDO;
    *iSemId = semget(IPC_PRIVATE, 1, iSemFlg);
    if (*iSemId < 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }

    c_arg.val = 1;
    rc = semctl(*iSemId, 0, SETVAL, c_arg);
    if (rc < 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    return MDIF_NORMAL;
}

int mdifCmnSemaDelete(int iSemId) {
    int rc;
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;

    INITIALIZE(rc);
    INITIALIZE(arg);

    rc = semctl(iSemId, 0, IPC_RMID, arg);
    if (rc < 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    return MDIF_NORMAL;
}
