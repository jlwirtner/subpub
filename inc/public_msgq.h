#ifndef PUBLIC_MSGQ_H
#define PUBLIC_MSGQ_H

#include <string>

namespace subpub {

class public_msgq {
public:
    public_msgq(std::string path);
    bool send(const std::string& message) const;
    std::string recv() const;
private:
    int id;
};

} // namespace subpub

#endif