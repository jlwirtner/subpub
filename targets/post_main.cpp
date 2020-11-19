#include <iostream>
#include "publisher.h"

int main() {

    subpub::publisher posty;

    posty.post("yo!");

    return 0;
}