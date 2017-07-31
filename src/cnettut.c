
#include "ae.h"
#include "anet.h"


void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask) {
    int cport, cfd;
    char cip[NET_IP_STR_LEN];
    UNUSED(el);
    UNUSED(mask);
    UNUSED(privdata);

    while(max--) {
        cfd = anetTcpAccept(server.neterr, fd, cip, sizeof(cip), &cport);
        if (cfd == ANET_ERR) {

        }

int main(int argc, char* argv[]){
    aeEventLoop *el = aeCreateEventLoop(10);
    char neterr[ANET_ERR_LEN];
    int ipfd = anetTcpServer(neterr, 8996, "0.0.0.0", 16);

    aeCreateFileEvent(el,ipfd,AE_READABLE,  acceptTcpHandler,NULL);

    return 0;
}
