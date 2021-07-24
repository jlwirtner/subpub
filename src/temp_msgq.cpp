#include "temp_msgq.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "common.h"

namespace {
    subpub::common::msg_t _msg {};
}

namespace subpub {

temp_msgq::temp_msgq() : tmp{} {
    key_t tmpKey {};
    int flags = IPC_CREAT;
    
    if ((tmpKey = ftok(tmp.get_path().c_str(), 1)) == -1) {
        throw std::runtime_error("Failed to get tmp key.");
    }

    if ((id = msgget(tmpKey, flags)) == -1) {
        throw std::runtime_error("Failed to get tmp msgQ.");
    }
}

std::string temp_msgq::recv() {
    if(msgrcv(id, &_msg, common::msgSize, 0, 0) < 0) {
        throw std::runtime_error("Failed to recv on temp_msgq.");
    }
    return _msg.mtext;
}

} //namespace subpub