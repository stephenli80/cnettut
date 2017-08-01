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

#define CNETTUT_ERR -1


#define NET_IP_STR_LEN 256

extern struct aServer server;

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
};




void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);



#endif //CNETTUT_SERVER_H
