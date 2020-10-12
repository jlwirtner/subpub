#ifndef SUBPUB_LISTENER_
#define SUBPUB_LISTENER_

#include <string>

namespace subpub {

using messageQId = int;

class listener {
public:
    listener();

    ~listener();

    int wait_for(const std::string& signal);

    static constexpr int ERROR_INVALID_MSG_LEN = -44;

private:
    messageQId listenerMsgQ;

    std::string tmpFileName;
};

} // namespace subpub
#endif 