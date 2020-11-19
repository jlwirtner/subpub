#include <iostream>
#include "subscriber.h"

int main() {

    subpub::subscriber signal;

    signal.wait_for("yo!");
    std::cout << "got signal!!" << std::endl;

    return 0;
}