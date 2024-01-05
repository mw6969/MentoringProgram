#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "SharedMemory.h"

SharedMemory::SharedMemory(const std::string& name)
{
    // Store shared memory name
    name_ = std::make_unique<std::string>(name);

    // Create and open a new object, or open an existing object
    int id_{shm_open(name.data(), O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG)};
    if (id_ < 0) {
        std::cerr << "shm_open failed with " << name << "\n";
    }

    // Set the size of the shared memory object
    if (ftruncate(id_, sizeof(std::queue<std::pair<std::string, size_t>>)) == -1) {
        std::cerr << "ftruncate failed with " << name << "\n";
    }

    // Map the shared memory object into the virtual address space of the calling process
    queue_ = (std::queue<std::pair<std::string, size_t>>*) mmap(NULL, sizeof(std::queue<std::pair<std::string, size_t>>), PROT_READ | PROT_WRITE, MAP_SHARED, id_, 0);
    if (queue_ == MAP_FAILED) {
        std::cerr << "mmap failed with " << name << "\n";
    }
}

SharedMemory::~SharedMemory()
{
    // Release the shared memory object
    shm_unlink(name_->data());
}

std::queue<std::pair<std::string, size_t>>* SharedMemory::get()
{
    return queue_;
}