#include "common.h"
#include "errno.h"
#include "stdio.h"
#include "stdlib.h"

namespace subpub {
namespace common {

    void printMsgQErr() {
        switch (errno) {
            case EACCES:
            perror("eaccess");
            exit(1);
            break;
            case EAGAIN:
            perror("eagain");
            exit(1);
            break;
            case EFAULT:
            perror("efault");
            exit(1);
            break;
            case EIDRM:
            perror("eidrm");
            exit(1);
            break;
            case EINTR:
            perror("eintr");
            exit(1);
            break;
            case EINVAL:
            perror("einval");
            exit(1);
            break;
            default:
            perror("idk");
            exit(1);
        }
    }

    void printFtokErr() {
        switch (errno) {
            case EACCES:
            perror("eaccess");
            exit(1);
            break;
            case EBADF:
            perror("ebadf");
            exit(1);
            break;
            case EFAULT:
            perror("efault");
            exit(1);
            break;
            case ELOOP:
            perror("eloop");
            exit(1);
            break;
            case ENAMETOOLONG:
            perror("enametoolong");
            exit(1);
            break;
            case ENOENT:
            perror("enoent");
            exit(1);
            break;
            case ENOMEM:
            perror("enomem");
            exit(1);
            break;
            case ENOTDIR:
            perror("enotdir");
            exit(1);
            break;
            case EOVERFLOW:
            perror("eoverflow");
            exit(1);
            break;
            default:
            perror("idk");
            exit(1);
        }
    }

} //namespace common
} //namespace subpub