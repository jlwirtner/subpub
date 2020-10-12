#include <iostream>
#include "Scheduler.h"

int main()
{
    subpub::Scheduler sched{};
    sched.start();
    
    std::cout << "Exiting main..." << std::endl;
    return 0;
}