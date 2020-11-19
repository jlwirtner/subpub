#ifndef SUBPUB_PUBLISHER_
#define SUBPUB_PUBLISHER_

#include <string>

namespace subpub {

using messageQId = int;

class publisher {
public:
    publisher();

    ~publisher();

    int post(const std::string& signal);

    static constexpr int ERROR_INVALID_MSG_LEN = -44;

private:
    messageQId signalMsgQ;
};

} // namespace subpub
#endif 