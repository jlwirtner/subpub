#include "common.h"
#include "listener.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <filesystem>

#include <fcntl.h>

namespace {
    subpub::common::msg_t listenerMsg;
    subpub::common::msg_t rcvMsg;

    namespace fs = std::__fs::filesystem;

    char delimiter[] = "***";
}

namespace subpub {

listener::listener() 
{
    key_t listenerKey;
    int flags = IPC_CREAT;

    if ((listenerKey   = ftok("keys/listeners", 1)) == -1) {
        common::printFtokErr();
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
        common::printFtokErr();
    }

    if ((tmpMsgQ = msgget(tmpKey, flags)) == -1) {
        std::cout << "failed to get tmp msgQ" << std::endl;
    } else {
        std::cout << tmpMsgQ << std::endl;
    }

    strcpy(listenerMsg.mtext+signal.length()+3, std::to_string(tmpMsgQ).c_str());
    std::cout << "message: " << listenerMsg.mtext << std::endl;

    if(msgsnd(listenerMsgQ, &listenerMsg, common::msgSize, 0) < 0) {
        std::cout << "error sending listener message" << std::endl;
        common::printMsgQErr();
    } else {
        std::cout << "sent listener message!" << std::endl;
        // return 0;
    }
    
    if(msgrcv(tmpMsgQ, &rcvMsg, common::msgSize, 0, 0) < 0) {
        std::cout << "error receiving listener message" << std::endl;
        common::printMsgQErr();
    } else {
        std::cout << "got a signal!" <<std::endl;
    }

    remove(filePath);
    return 0;
}

} // namespace subpub
