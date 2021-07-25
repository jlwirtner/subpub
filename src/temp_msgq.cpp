#include "temp_msgq.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include <sstream>

#include "common.h"

namespace {
    subpub::common::msg_t _msg {};
}

namespace subpub {

temp_msgq::temp_msgq() : tmp{} {
    key_t tmpKey {};
    int flags = IPC_CREAT;
    
    if ((tmpKey = ftok(tmp.get_path().c_str(), 1)) == -1) {
        std::stringstream error {};
        error << "Failed to get tmp key: " << errno;
        throw std::runtime_error(error.str());
    }

    if ((id = msgget(tmpKey, flags)) == -1) {
        std::stringstream error {};
        error << "Failed to get tmp msgQ: " << errno;
        throw std::runtime_error(error.str());
    }
}

std::string temp_msgq::recv() {
    if(msgrcv(id, &_msg, common::msgSize, 0, 0) < 0) {
        throw std::runtime_error("Failed to recv on temp_msgq.");
    }
    return _msg.mtext;
}

} //namespace subpub