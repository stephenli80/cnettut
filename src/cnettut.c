
#include "ae.h"
#include "anet.h"
#include "server.h"



int main(int argc, char* argv[]){
    aeEventLoop *el = aeCreateEventLoop(10);
    char neterr[ANET_ERR_LEN];
    int ipfd = anetTcpServer(neterr, 8996, "0.0.0.0", 16);

    aeCreateFileEvent(el,ipfd,AE_READABLE,  acceptTcpHandler,NULL);

    return 0;
}
