#include "xt_headers.h"

#define KEYVALLEN 100

int get_property(char *profile, char *AppName, char *KeyName, char *KeyVal) {
    char appname[32], keyname[32];
    char *buf, *c;
    char buf_i[KEYVALLEN], buf_o[KEYVALLEN];
    FILE *fp;
    int found = 0; /* 1 AppName 2 KeyName */
    if ((fp = fopen(profile, "r")) == NULL) {
        ERRORLOG(__FUNCTION__);
        return MDIF_ERROR;
    }
    fseek(fp, 0, SEEK_SET);
    memset(appname, 0, sizeof(appname));
    sprintf(appname, "[%s]", AppName);

    while (!feof(fp) && fgets(buf_i, KEYVALLEN, fp) != NULL) {
        l_trim(buf_o, buf_i);
        if (strlen(buf_o) <= 0)
            continue;
        buf = NULL;
        buf = buf_o;

        if (found == 0) {
            if (buf[0] != '[') {
                continue;
            } else if (strncmp(buf, appname, strlen(appname)) == 0) {
                found = 1;
                continue;
            }

        } else if (found == 1) {
            if (buf[0] == '#') {
                continue;
            } else if (buf[0] == '[') {
                break;
            } else {
                if ((c = (char *) strchr(buf, '=')) == NULL)
                    continue;
                memset(keyname, 0, sizeof(keyname));

                sscanf(buf, "%[^=|^ |^\t]", keyname);
                if (strcmp(keyname, KeyName) == 0) {
                    sscanf(++c, "%[^\n]", KeyVal);
                    char *keyVal_o = (char *) malloc(strlen(KeyVal) + 1);
                    if (keyVal_o != NULL) {
                        memset(keyVal_o, '\0', strlen(KeyVal) + 1);
                        trim_string(keyVal_o, KeyVal);
                        if (keyVal_o && strlen(keyVal_o) > 0)
                            strcpy(KeyVal, keyVal_o);
                        free(keyVal_o);
                        keyVal_o = NULL;
                    }
                    found = 2;
                    break;
                } else {
                    continue;
                }
            }
        }
    }
    fclose(fp);
    if (found == 2)
        return MDIF_NORMAL;
    else
        return MDIF_ERROR;
}
