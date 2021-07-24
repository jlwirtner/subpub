#include "temp_file.h"

#include <fcntl.h>

namespace subpub {

temp_file::temp_file() : path {""} {
    char fileNameTemp[] = "tmpXXXXXX";
    auto tmp = mkstemp(fileNameTemp);

    char filePath[100];
    fcntl(tmp, F_GETPATH, filePath);

    path = filePath;
}

temp_file::~temp_file() {
    remove(path.c_str());
}

} // namespace subpub
