#include "publisher.h"
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

namespace {
    const int msgSize = 50;
    typedef struct msg {
        long mtype = 1;
        char mtext[msgSize];
    } msg_t;

    msg_t signalMsg;
}

namespace subpub {

publisher::publisher() 
{
    key_t signalKey;
    int flags = IPC_CREAT;

    if ((signalKey   = ftok("keys/signal", 1)) == -1) {
        std::cout << "failed to get signal key" << std::endl;
    }

    if ((signalMsgQ = msgget(signalKey, flags)) == -1) {
        std::cout << "failed to get signal msgQ" << std::endl;
    }
}

publisher::~publisher()
{

}

int publisher::post(const std::string& signal)
{
    if (signal.length() > 50) {
        return ERROR_INVALID_MSG_LEN;
    }
    strcpy(signalMsg.mtext, signal.c_str());
    if(msgsnd(signalMsgQ, &signalMsg, msgSize, 0) < 0) {
        std::cout << "error sending signal message" << std::endl;
        switch (errno) {
            case EACCES:
            perror("eaccess");
            exit(1);
            break;
            case EAGAIN:
            perror("eagain");
            exit(1);
            break;
            case EFAULT:
            perror("efault");
            exit(1);
            break;
            case EIDRM:
            perror("eidrm");
            exit(1);
            break;
            case EINTR:
            perror("eintr");
            exit(1);
            break;
            case EINVAL:
            perror("einval");
            exit(1);
            break;
            default:
            perror("idk");
            exit(1);
        }
    } else {
        std::cout << "sent signal message!" << std::endl;
        return 0;
    }
}

} // namespace subpub