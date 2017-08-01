//
// Created by 李 on 2017/7/30.
//

#ifndef CNETTUT_SERVER_H
#define CNETTUT_SERVER_H

#include "sds.h"
#include "ae.h"
#include "anet.h"

#define C_OK                    0
#define C_ERR                   -1

#define REDIS_LOG_RAW (1<<10) /* Modifier to log without timestamp */

/* Log levels */
#define CNETTUT_DEBUG 0
#define CNETTUT_VERBOSE 1
#define CNETTUT_NOTICE 2
#define CNETTUT_WARNING 3
#define CNETTUT_LOG_RAW (1<<10) /* Modifier to log without timestamp */
#define CNETTUT_DEFAULT_VERBOSITY CNETTUT_NOTICE

#define CNETTUT_ERR -1

#define NET_IP_STR_LEN 256



typedef struct aClient {
    int fd;


    sds querybuf;
} aClient;

struct aServer {
    char neterr[ANET_ERR_LEN];
    int port ;
    int ipfd;

    aeEventLoop *el;

    aClient *current_client;
    char *logfile;
    int verbosity;
    int syslog_enabled;
};



#ifdef __GNUC__
void cnettutLog(int level, const char *fmt, ...)
__attribute__((format(printf, 2, 3)));
#else
void redisLog(int level, const char *fmt, ...);
#endif
void cnettutLogRaw(int level, const char *msg);






#endif //CNETTUT_SERVER_H
