//
// Created by 李 on 2017/7/30.
//

#ifndef CNETTUT_SERVER_H
#define CNETTUT_SERVER_H


void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);

#endif //CNETTUT_SERVER_H
