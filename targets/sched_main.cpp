#include <iostream>
#include "Scheduler.h"

int main()
{
    subpub::scheduler sched{};
    sched.start();
    
    std::cout << "Exiting main..." << std::endl;
    return 0;
}