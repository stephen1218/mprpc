#pragma once

#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>

//生产者消费者模型
template<typename T>
class LockQueue {
private:
    std::queue<T> que_;
    std::mutex mtx_;
    std::condition_variable cv_;
public:
    void push(const T& data) {
        std::unique_lock<std::mutex> lck(mtx_);
        que_.push(data);
        cv_.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lck(mtx_);
        while(que_.empty()) {
            cv_.wait(lck);
        }
        T data = que_.front();
        que_.pop();
        return data;
    }
};