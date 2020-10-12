#include "listener.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <filesystem>

#include <fcntl.h>

namespace {
    const int msgSize = 50;
    typedef struct msg {
        long mtype = 1;
        char mtext[msgSize];
    } msg_t;

    msg_t listenerMsg;
    msg_t rcvMsg;

    namespace fs = std::__fs::filesystem;

    char delimiter[] = "***";
}

namespace subpub {

listener::listener() 
{
    key_t listenerKey;
    int flags = IPC_CREAT;

    if ((listenerKey   = ftok("keys/listeners", 1)) == -1) {
        std::cout << "failed to get listener key" << std::endl;
        switch (errno) {
            case EACCES:
            perror("eaccess");
            exit(1);
            break;
            case EBADF:
            perror("ebadf");
            exit(1);
            break;
            case EFAULT:
            perror("efault");
            exit(1);
            break;
            case ELOOP:
            perror("eloop");
            exit(1);
            break;
            case ENAMETOOLONG:
            perror("enametoolong");
            exit(1);
            break;
            case ENOENT:
            perror("enoent");
            exit(1);
            break;
            case ENOMEM:
            perror("enomem");
            exit(1);
            break;
            case ENOTDIR:
            perror("enotdir");
            exit(1);
            break;
            case EOVERFLOW:
            perror("eoverflow");
            exit(1);
            break;
            default:
            perror("idk");
            exit(1);
        }
    } 
    
    if ((listenerMsgQ = msgget(listenerKey, flags)) == -1) {
        std::cout << "failed to get listener msgQ" << std::endl;
    }
}

listener::~listener()
{
    //Delete tmp file! i think
}

int listener::wait_for(const std::string& signal)
{
    
    key_t tmpKey{};
    int tmpMsgQ{};
    int flags = IPC_CREAT;

    char fileNameTemp[] = "tmpXXXXXX";
    auto tmp = mkstemp(fileNameTemp);

    char filePath[100];
    fcntl(tmp, F_GETPATH, filePath);

    std::cout << "file: " << filePath <<std::endl;


    if (signal.length() > 48) {
        return ERROR_INVALID_MSG_LEN;
    }
    strcpy(listenerMsg.mtext, signal.c_str());
    strcpy(listenerMsg.mtext+signal.length(), delimiter);
    
    if ((tmpKey = ftok(filePath, 1)) == -1) {
        std::cout << "failed to get tmp key: " << tmpFileName <<std::endl;
    }

    if ((tmpMsgQ = msgget(tmpKey, flags)) == -1) {
        std::cout << "failed to get tmp msgQ" << std::endl;
    } else {
        std::cout << tmpMsgQ << std::endl;
    }

    strcpy(listenerMsg.mtext+signal.length()+3, std::to_string(tmpMsgQ).c_str());
    std::cout << "message: " << listenerMsg.mtext << std::endl;

    if(msgsnd(listenerMsgQ, &listenerMsg, msgSize, 0) < 0) {
        std::cout << "error sending listener message" << std::endl;
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
        std::cout << "sent listener message!" << std::endl;
        // return 0;
    }
    
    if(msgrcv(tmpMsgQ, &rcvMsg, msgSize, 0, 0) < 0) {
        std::cout << "error receiving listener message" << std::endl;
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
        std::cout << "got a signal!" <<std::endl;
    }

    remove(filePath);
    return 0;
}

} // namespace subpub