#ifndef TEMP_FILE_H
#define TEMP_FILE_H

#include <string>

namespace subpub {

class temp_file {
public:
    temp_file();
    ~temp_file();

    temp_file(const temp_file&) = delete;
    temp_file(temp_file&&) = delete;
    temp_file& operator=(const temp_file&) = delete;
    temp_file& operator=(temp_file&&) = delete;

    inline std::string get_path() const { return path; };

private:
    std::string path;
};

} // namespace subpub

#endif