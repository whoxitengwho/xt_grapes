#ifndef _common_h_
#define _common_h_


#define  THREADTBL_OCCURS            100
#define  MDIF_PROC_MAX               10

#define  MDIF_EXEC_END               '0'      /* 启动终止 */
#define  MDIF_EXEC_START             '1'      /* 启动开始 */
#define  MDIF_FLG_OFF                '0'      /* 线程终止标记：不终止 */
#define  MDIF_FLG_ON                 '1'      /* 线程终止标记: 终止 */
#define  MDIF_FLG_ERR                '2'      /* 线程终止标记：异常 */


#define  MDIF_THREAD_STS_ERROR        '1'     /* 线程启动错误 */
#define  MDIF_THREAD_STS_NORMAL        '0'     /* 线程正常 */

#define  MDIF_THREAD_STS_NOCREATE      '0'     /* 未生成 */
#define  MDIF_THREAD_STS_CREATING      '1'     /* 创建中 */
#define  MDIF_THREAD_STS_CREATED       '2'     /* 创建好了 */

#define  MDIF_THREAD_STS_NOTUSE        '0'     /* 等待使用 */
#define  MDIF_THREAD_STS_USED          '1'     /* 使用中 */
#define  MDIF_THREAD_STS_INTROUBLE     '1'     /* 有错误 */


#define  MDIF_NORMAL                  0       /* 函数实行成功 */
#define  MDIF_ERROR                  -1       /* 函数实行失败 */


#define  INITIALIZE(var)              memset(&var, 0x00, sizeof var)
#define  ERRORLOG(var)      printf("error in function %s: %s:  Line: [%d]: errno=[%d]: var=[%s]\n", \
                                                                    __FUNCTION__,\
                                                                    __FILE__,    \
                                                                    __LINE__,    \
                                                                    errno   ,     \
                                                                    var)


#define MDIF_CONF_PRCSHRMEM_PATH "shared"  /* 共享内存在硬盘映射地址 */

#define MDIF_CONF_PROCESSTERM_TRYCOUNT 60  /* 确认所有进程终了: 次数 */

#define MDIF_CONF_THREADTERM_TRYCOUNT 30   /* 确认当前进程所有线程终了:次数 */

/* MD.interface */
typedef struct tagCONF_MDIF_INFO {
    char szComProcess_Name[100];      /* 工作进程名 */
    int iComProcess_Counter;          /* 工作进程启动数 */
    int iExecSts_Retry_Counter;       /* 确认进程启动:次数 */
    int iExecThread_Retry_Counter;    /* 确认线程启动:次数 */
    int szThread_SemId;               /* 线程P/V ID */
    int iThreadStrCheckTime;          /* 线程启动确认时间 */
    int iMsgComWaitTime;              /* 主监听工作子进程间隔时间:微妙 */
    char cPortString[200];             /* 所有监听端口*/
} CONF_MDIF_INFO;

/* 进程单元信息 */
typedef struct tagCOMPRC_INFO {
    int iProcNo;                         /* 工作进程番号 */
    pid_t pPid;                           /* 工作进程ID */
    char cProcSts;                        /* 进程启动状态 */
    char cSaveSts;                        /* 进程生存状态 */
    char cExitSts;                        /* 进程终了标记 */
    int iPrcThread_Cnt;                   /* 启动线程数 */
} COMPRC_INFO;

typedef struct tagTHREAD_TBL {
    char cApp_Id[30];                /* 程序ID */
    int iThread_No;                 /* 线程号 */
    pthread_t thread_Id;              /* 线程ID */
    char cStart_Status;              /* 线程启动状态 */
    char cThread_Status;             /* 线程启动状态 */
    int iComProcess_Id;             /* 工作进程ID */
    char cThreadTermFlag;            /* 线程终止标记 */
    int iComProcess_No;               /* 工作进程番号 */
} THREAD_TBL;


/* 共享内存 */
typedef struct tagCONF_PRCSHRMEM {
    CONF_MDIF_INFO MdIf_Info;                   /* MD.interface信息 */
    THREAD_TBL Thread_Tbl[THREADTBL_OCCURS];    /* 线程表 */
    COMPRC_INFO ComPrc_Tbl[MDIF_PROC_MAX];      /* 进程表 */
    char PmgrTermFlag;                          /* 进程终止标记 */
} CONF_PRCSHRMEM_INFO;


#endif