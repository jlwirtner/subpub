#ifndef COMMON_
#define COMMON_

namespace subpub {
namespace common {

    const int msgSize = 50;
    typedef struct msg {
        long mtype = 1;
        char mtext[msgSize];
    } msg_t;

    void printMsgQErr();

    void printFtokErr();

} //namespace common
} //namespace subpub

#endif