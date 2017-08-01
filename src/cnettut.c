
#include "ae.h"
#include "anet.h"
#include "server.h"


/*================================= Globals ================================= */

/* Global vars */
struct aServer server; /* server global state */

int main(int argc, char* argv[]){

    server.port = 8996;

    int ipfd = anetTcpServer(server.neterr, 8996, "0.0.0.0", 16);

    if(ipfd == AE_ERR){

        //todo , log
        return AE_ERR;
    }

    anetNonBlock(NULL,ipfd);

    server.ipfd =ipfd;


    aeEventLoop *el = aeCreateEventLoop(10);

    server.el =el;


    aeCreateFileEvent(el,ipfd,AE_READABLE,  acceptTcpHandler,NULL);




    return 0;
}
