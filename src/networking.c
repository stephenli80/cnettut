/*
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/uio.h>
#include <sys/unistd.h>
#include <sys/errno.h>
#include <math.h>

#include "cnettut.h"
#include "server.h"


#include "zmalloc.h"

#include "sds.h"





void readQueryFromClient(aeEventLoop *el, int fd, void *privdata, int mask) {
    aClient *c = (aClient*) privdata;
    int nread, readlen;

    CNETTUT_NOTUSED(el);
    CNETTUT_NOTUSED(mask);

    server.current_client = c;
    readlen = CNETTUT_IOBUF_LEN;

    nread = read(fd, c->querybuf, readlen);
    if (nread == -1) {
        if (errno == EAGAIN) {
            nread = 0;
        } else {
           // redisLog(REDIS_VERBOSE, "Reading from client: %s",strerror(errno));
            freeClient(c);
            return;
        }
    } else if (nread == 0) {
        //redisLog(REDIS_VERBOSE, "Client closed connection");
        freeClient(c);
        return;
    }
    if (nread) {
        sdsIncrLen(c->querybuf,nread);

    } else {
        //todo why ,re judge 0
        server.current_client = NULL;
        return;
    }

    processInputBuffer(c);
    server.current_client = NULL;
}



aClient *createClient(int fd) {
    aClient *c = zmalloc(sizeof(aClient));

    /* passing -1 as fd it is possible to create a non connected client.
     * This is useful since all the Redis commands needs to be executed
     * in the context of a client. When commands are executed in other
     * contexts (for instance a Lua script) we need a non connected client. */
    if (fd != -1) {
        anetNonBlock(NULL, fd);
        anetEnableTcpNoDelay(NULL, fd);
        if (aeCreateFileEvent(server.el, fd, AE_READABLE,
                              readQueryFromClient, c) == AE_ERR) {
            close(fd);
            zfree(c);
            return NULL;
        }
    }
}

void freeClient(aClient *c) {


    /* If this is marked as current client unset it */
    if (server.current_client == c) server.current_client = NULL;



    /* Free the query buffer */
    sdsfree(c->querybuf);
    c->querybuf = NULL;

    zfree(c);
}

void processInputBuffer(aClient *c) {
    /* Keep processing while there is something in the input buffer */
    while (sdslen(c->querybuf)) {
           //todo,log

        sdsclear(c->querybuf);

    }
}


void acceptCommonHandler(int fd, int flags) {
    aClient *c;
    if ((c = createClient(fd)) == NULL) {
        //todo log
        close(fd); /* May be already closed, just ignore errors */
        return;
    }

    //c->flags |= flags;
}

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    int cport, cfd;
    char cip[NET_IP_STR_LEN];


    cfd = anetTcpAccept(server.neterr, fd, cip, sizeof(cip), &cport);
    if (cfd == ANET_ERR) {
        //todo, log
        return;
    }


    acceptCommonHandler(cfd,0);


}
