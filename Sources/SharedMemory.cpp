#include <iostream>

#include "SharedMemory.h"

SharedMemory::~SharedMemory()
{
    std::cout << "~SharedMemory\n";
}

std::shared_ptr<SharedMemory> SharedMemory::create()
{
    return std::shared_ptr<SharedMemory>{new (&data_) SharedMemory{}, [](SharedMemory* sharedMemory){
        //sharedMemory->~SharedMemory();
    }};
}