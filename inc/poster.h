#ifndef SUBPUB_POSTER_
#define SUBPUB_POSTER_

#include <string>

namespace subpub {

using messageQId = int;

class poster {
public:
    poster();

    ~poster();

    int post(const std::string& signal);

    static constexpr int ERROR_INVALID_MSG_LEN = -44;

private:
    messageQId signalMsgQ;
};

} // namespace subpub
#endif 