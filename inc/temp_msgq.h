#ifndef TEMP_MSGQ_H
#define TEMP_MSGQ_H

#include "temp_file.h"

namespace subpub {

class temp_msgq {
public:
    temp_msgq();
    inline int get_id() { return id; };
    std::string recv();
private:
    temp_file tmp;
    int id;
};

} // namespace subpub

#endif