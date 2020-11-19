#include "common.h"
#include "subscriber.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <filesystem>

#include <fcntl.h>

namespace {
    subpub::common::msg_t subscriberMsg;
    subpub::common::msg_t rcvMsg;

    namespace fs = std::__fs::filesystem;

    char delimiter[] = "***";
}

namespace subpub {

subscriber::subscriber() 
{
    key_t subscriberKey;
    int flags = IPC_CREAT;

    if ((subscriberKey   = ftok("keys/subscribers", 1)) == -1) {
        common::printFtokErr();
    } 
    
    if ((subscriberMsgQ = msgget(subscriberKey, flags)) == -1) {
        std::cout << "failed to get subscriber msgQ" << std::endl;
    }
}

subscriber::~subscriber()
{
    //Delete tmp file! i think
}

int subscriber::wait_for(const std::string& signal)
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
    strcpy(subscriberMsg.mtext, signal.c_str());
    strcpy(subscriberMsg.mtext+signal.length(), delimiter);
    
    if ((tmpKey = ftok(filePath, 1)) == -1) {
        std::cout << "failed to get tmp key: " << tmpFileName <<std::endl;
        common::printFtokErr();
    }

    if ((tmpMsgQ = msgget(tmpKey, flags)) == -1) {
        std::cout << "failed to get tmp msgQ" << std::endl;
    } else {
        std::cout << tmpMsgQ << std::endl;
    }

    strcpy(subscriberMsg.mtext+signal.length()+3, std::to_string(tmpMsgQ).c_str());
    std::cout << "message: " << subscriberMsg.mtext << std::endl;

    if(msgsnd(subscriberMsgQ, &subscriberMsg, common::msgSize, 0) < 0) {
        std::cout << "error sending subscriber message" << std::endl;
        common::printMsgQErr();
    } else {
        std::cout << "sent subscriber message!" << std::endl;
        // return 0;
    }
    
    if(msgrcv(tmpMsgQ, &rcvMsg, common::msgSize, 0, 0) < 0) {
        std::cout << "error receiving subscriber message" << std::endl;
        common::printMsgQErr();
    } else {
        std::cout << "got a signal!" <<std::endl;
    }

    remove(filePath);
    return 0;
}

} // namespace subpub
