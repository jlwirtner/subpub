#include "publisher.h"

namespace subpub {

publisher::publisher() : signalMsgQ{"keys/signal"}
{

}

int publisher::post(const std::string& signal)
{
    if (signal.length() > 50)
        return ERROR_INVALID_MSG_LEN;

    if(signalMsgQ.send(signal))
        return 0;

    return -1;
}

} // namespace subpub