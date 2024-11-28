#ifndef _PROCESSOR_
#define _PROCESSOR_

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

template <typename Derived, typename DataType> class Processor {
public:
  void ProcessData() {
    static_cast<Derived *>(this)->Initialize();
    while (!static_cast<Derived *>(this)->IsDone()) {
      if (!static_cast<Derived *>(this)->ReadyToProcessData()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        continue;
      }
      static_cast<Derived *>(this)->ProcessDataImpl();
    }
    static_cast<Derived *>(this)->Finalize();
    this->done_ = true;
  }

  bool WaitNextData() { return dataQueue_.size() < 10; }

  bool NotifyComplete() { return done_; }

  void Push(DataType &&data) {
    std::lock_guard<std::mutex> lock(mutex_);
    dataQueue_.push(data);
    cv_.notify_one();
  }

  DataType Pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !dataQueue_.empty(); });
    auto data = dataQueue_.front();
    dataQueue_.pop();
    return data;
  }

  bool Empty() { return dataQueue_.empty(); }

protected:
  std::queue<DataType> dataQueue_;
  std::mutex mutex_;
  std::condition_variable cv_;
  bool done_ = false;
};

#endif