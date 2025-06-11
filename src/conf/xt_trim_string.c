#include "xt_headers.h"

/*   删除左边的空格   */
char *l_trim(char *szOutput, const char *szInput) {
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    for (NULL; *szInput != '\0' && isspace(*szInput); ++szInput) { ;
    }
    return strcpy(szOutput, szInput);
}

/*   删除右边的空格   */
char *r_trim(char *szOutput, const char *szInput) {
    char *p = NULL;
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    strcpy(szOutput, szInput);
    for (p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p) { ;
    }
    *(++p) = '\0';
    return szOutput;
}

/*   删除两边的空格   */
char *trim_string(char *szOutput, const char *szInput) {
    char *p = NULL;
    assert(szInput != NULL);
    assert(szOutput != NULL);
    l_trim(szOutput, szInput);
    for (p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p) { ;
    }
    *(++p) = '\0';
    return szOutput;
}
 