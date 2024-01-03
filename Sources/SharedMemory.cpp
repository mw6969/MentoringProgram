#include <iostream>

#include "SharedMemory.h"

SharedMemory::SharedMemory(const std::string &name)
{
    name_ = std::make_unique<std::string>(name);
    conditionVariable_ = std::make_shared<pthread_cond_t>();
    mutex_ = std::make_shared<pthread_mutex_t>();
}

SharedMemory::~SharedMemory()
{
    // Destroy mutex with condition
    pthread_mutex_destroy(mutex_.get());
    pthread_cond_destroy(conditionVariable_.get());

    // Remove a shared memory object name
    shm_unlink(name_->data());
}

void SharedMemory::push(const std::string &data, const size_t size)
{
    open();
    fileTruncate();
    memoryMap(data, size);
}

std::shared_ptr<pthread_cond_t> SharedMemory::getConditionVariable() const
{
    return conditionVariable_;
}

std::shared_ptr<pthread_mutex_t> SharedMemory::getMutex() const
{
    return mutex_;
}

bool SharedMemory::empty()
{
    return queue_.empty();
}

std::string SharedMemory::itemBuf()
{
    return queue_.front().first;
}

std::size_t SharedMemory::itemSize()
{
    return queue_.front().second;
}

void SharedMemory::pop()
{
    queue_.pop();
}

void SharedMemory::open()
{
    // Create and open a new object, or open an existing object
    fileDescriptor_ = shm_open(name_->data(), O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fileDescriptor_ == -1) {
        std::cerr << "An error occurred in the shared memory object creation stage\n";
    }
}

void SharedMemory::fileTruncate()
{
    // Set the size of the shared memory object
    if (ftruncate(fileDescriptor_, sizeof(*this)) == -1) {
        std::cerr << "An error occurred in the setting size of the shared memory object\n";
    }
}

void SharedMemory::memoryMap(const std::string &data, const size_t size)
{
    // Map the shared memory object into the virtual address space of the calling process
    SharedMemory* shmp{(SharedMemory*)(mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor_, 0))};
    if (shmp == MAP_FAILED) {
        std::cerr << "An error occurred when mapping the shared memory object into the virtual address space of the calling process\n";
    }

    queue_.push(std::make_pair(data, size));
}
