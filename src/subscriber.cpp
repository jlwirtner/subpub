#include "common.h"
#include "subscriber.h"
#include "temp_msgq.h"

namespace {
    std::string createMessage(const std::string& signal, int msg_id);
}

namespace subpub {

subscriber::subscriber() : subscriberMsgQ{"keys/subscribers"}
{

}

int subscriber::wait_for(const std::string& signal)
{
    if (signal.length() > 48)
        return ERROR_INVALID_MSG_LEN;

    temp_msgq tmpMsgQ {};
    std::string message {createMessage(signal, tmpMsgQ.get_id())};

    // inform scheduler scheduler that this msgQ is waiting for
    // the given signal
    if(!subscriberMsgQ.send(message))
        throw std::runtime_error("Failed to send to subscriber msgQ.");
    
    // Block until signal is published
    tmpMsgQ.recv();

    return 0;
}

} // namespace subpub

namespace {

std::string createMessage(const std::string& signal, int msg_id) {
    static const char delimiter[] = "***";
    std::string output {signal};
    output += delimiter;
    output += std::to_string(msg_id);
    return output;
}

}
