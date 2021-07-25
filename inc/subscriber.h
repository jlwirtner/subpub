#ifndef SUBPUB_SUBSCRIBER_
#define SUBPUB_SUBSCRIBER_

#include <string>

#include "public_msgq.h"

namespace subpub {

using messageQId = int;

class subscriber {
public:
    subscriber();

    int wait_for(const std::string& signal);

    static constexpr int ERROR_INVALID_MSG_LEN = -44;

private:
    public_msgq subscriberMsgQ;
};

} // namespace subpub
#endif 