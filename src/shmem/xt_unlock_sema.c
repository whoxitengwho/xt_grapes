#include "xt_headers.h"

int unlock_sema(int iSemID) {
    int rc;
    struct sembuf sb;
    INITIALIZE(rc);
    INITIALIZE(sb);

    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = SEM_UNDO;

    rc = semop(iSemID, &sb, 1);
    if (rc < 0) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    return MDIF_NORMAL;
}