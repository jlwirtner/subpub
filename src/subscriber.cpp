#include "common.h"
#include "subscriber.h"
#include "temp_msgq.h"
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

    char delimiter[] = "***";

    void prepareMsg(const std::string& signal, int msg_id);
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
    if (signal.length() > 48) {
        return ERROR_INVALID_MSG_LEN;
    }

    temp_msgq tmpMsgQ {};

    prepareMsg(signal, tmpMsgQ.get_id());

    if(msgsnd(subscriberMsgQ, &subscriberMsg, common::msgSize, 0) < 0) {
        std::cout << "error sending subscriber message" << std::endl;
        common::printMsgQErr();
    } else {
        std::cout << "sent subscriber message!" << std::endl;
        // return 0;
    }

    if(tmpMsgQ.recv() != "")
        std::cout << "got a signal!\n";

    return 0;
}

} // namespace subpub

namespace {

void prepareMsg(const std::string& signal, int msg_id) {
    strcpy(subscriberMsg.mtext, signal.c_str());
    strcpy(subscriberMsg.mtext+signal.length(), delimiter);
    strcpy(subscriberMsg.mtext+signal.length()+3, std::to_string(msg_id).c_str());
}

}
