#ifndef SUBPUB_PUBLISHER_
#define SUBPUB_PUBLISHER_

#include <string>

#include "public_msgq.h"

namespace subpub {

using messageQId = int;

class publisher {
public:
    publisher();

    int post(const std::string& signal);

    static constexpr int ERROR_INVALID_MSG_LEN = -44;

private:
    public_msgq signalMsgQ;
};

} // namespace subpub
#endif 