#include <iostream>
#include "listener.h"

int main() {

    subpub::listener signal;

    signal.wait_for("yo!");
    std::cout << "got signal!!" << std::endl;

    return 0;
}