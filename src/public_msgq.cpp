#include "public_msgq.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <iostream>

#include "common.h"

namespace {
    subpub::common::msg_t _msg {};
}

namespace subpub {

public_msgq::public_msgq(std::string path) : id{} {
    key_t tmpKey {};
    int flags = IPC_CREAT;
    
    if ((tmpKey = ftok(path.c_str(), 1)) == -1) {
        throw std::runtime_error("Failed to get public key.");
    }

    if ((id = msgget(tmpKey, flags)) == -1) {
        throw std::runtime_error("Failed to get public msgQ.");
    }
}

std::string public_msgq::recv() const {
    if(msgrcv(id, &_msg, common::msgSize, 0, 0) < 0) {
        throw std::runtime_error("Failed to recv on temp_msgq.");
    }
    return _msg.mtext;
}

bool public_msgq::send(const std::string& message) const {
    strcpy(_msg.mtext, message.c_str());

    if(msgsnd(id, &_msg, common::msgSize, 0) < 0)
        return false;

    return true;
}

} // namespace subpub

