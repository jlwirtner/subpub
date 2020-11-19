#ifndef SUBPUB_SUBSCRIBER_
#define SUBPUB_SUBSCRIBER_

#include <string>

namespace subpub {

using messageQId = int;

class subscriber {
public:
    subscriber();

    ~subscriber();

    int wait_for(const std::string& signal);

    static constexpr int ERROR_INVALID_MSG_LEN = -44;

private:
    messageQId subscriberMsgQ;

    std::string tmpFileName;
};

} // namespace subpub
#endif 