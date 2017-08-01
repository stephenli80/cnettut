
#include "ae.h"
#include "anet.h"
#include "server.h"


extern  void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);

/*================================= Globals ================================= */

/* Global vars */
struct aServer server; /* server global state */

int main(int argc, char* argv[]){

    server.port = 8996;

    server.logfile = "cnettut.log";
    server.verbosity = CNETTUT_DEBUG;

    server.syslog_enabled =0;

    int ipfd = anetTcpServer(server.neterr, server.port, "0.0.0.0", 16);

    //cnettutLog(CNETTUT_NOTICE,"bind to port :%s", server.port);

    if(ipfd == AE_ERR){

        cnettutLog(CNETTUT_WARNING,"failed bind :%s", server.neterr);
        return AE_ERR;
    }

    anetNonBlock(NULL,ipfd);

    server.ipfd =ipfd;


    aeEventLoop *el = aeCreateEventLoop(10);

    server.el =el;


    aeCreateFileEvent(el,ipfd,AE_READABLE,  acceptTcpHandler,NULL);


    aeMain(server.el);
    aeDeleteEventLoop(server.el);
    return 0;



}
