
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "ae.h"
#include "anet.h"
#include "server.h"


extern  void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);

/*================================= Globals ================================= */

/* Global vars */
struct aServer server; /* server global state */


static void sigShutdownHandler(int sig) {
    char *msg;

    switch (sig) {
        case SIGINT:
            msg = "Received SIGINT scheduling shutdown...";
            break;
        case SIGTERM:
            msg = "Received SIGTERM scheduling shutdown...";
            break;
        default:
            msg = "Received shutdown signal, scheduling shutdown...";
    };

    /* SIGINT is often delivered via Ctrl+C in an interactive session.
     * If we receive the signal the second time, we interpret this as
     * the user really wanting to quit ASAP without waiting to persist
     * on disk. */

    if(server.ipfd > 0)
       close(server.ipfd);

    cnettutLog(CNETTUT_WARNING, "You insist... exiting now: %s",msg);

   exit(12);


}

void setupSignalHandlers(void) {
    struct sigaction act;

    /* When the SA_SIGINFO flag is set in sa_flags then sa_sigaction is used.
     * Otherwise, sa_handler is used. */
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = sigShutdownHandler;
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGINT, &act, NULL);

#ifdef HAVE_BACKTRACE
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER | SA_RESETHAND | SA_SIGINFO;
    act.sa_sigaction = sigsegvHandler;
    sigaction(SIGSEGV, &act, NULL);
    sigaction(SIGBUS, &act, NULL);
    sigaction(SIGFPE, &act, NULL);
    sigaction(SIGILL, &act, NULL);
#endif
    return;
}


void daemonize(void) {
    int fd;

    if (fork() != 0) exit(0); /* parent exits */
    setsid(); /* create a new session */

    /* Every output goes to /dev/null. If Redis is daemonized but
     * the 'logfile' is set to 'stdout' in the configuration file
     * it will not log at all. */
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) close(fd);
    }
}


int main(int argc, char* argv[]){

    server.port = 8996;

    server.logfile = "cnettut1.log";
    server.verbosity = CNETTUT_DEBUG;

    server.syslog_enabled =0;
    server.shutdown_asap =1;

    server.ipfd = AE_ERR;

   // signal(SIGHUP, SIG_IGN);
   // signal(SIGPIPE, SIG_IGN);

    setupSignalHandlers();

    //daemonize();

    cnettutLog(CNETTUT_NOTICE,"begin");

    int ipfd = anetTcpServer(server.neterr, server.port, "0.0.0.0", 16);

    cnettutLog(CNETTUT_NOTICE,"bind to port :%d", server.port);

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
