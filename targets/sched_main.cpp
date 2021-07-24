#include <iostream>
#include "Scheduler.h"

int main()
{
    subpub::scheduler sched{};
    sched.start();
    
    std::cin.get();
    std::cout << "Exiting main..." << std::endl;
    return 0;
}