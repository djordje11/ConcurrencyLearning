#include <mutex>
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue() {}
    threadsafe_queue(const threadsafe_queue &other)
    {
        std::lock_guard<std::mutex> lock(other.mut);
        data_queue = other.data_queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> uni_lock(mut);
        data_cond.wait(uni_lock, [this](){return !data_queue.empty();});
        value = data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> uni_lock(mut);
        data_cond.wait(uni_lock, [this](){return !data_queue.empty();});
        auto ptr = std::make_shared<T>(data_queue.front());
        data_queue.pop();
        return ptr;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> uni_lock(mut);
        if(data_queue.empty()) return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> uni_lock(mut);
        if(data_queue.empty()) return {};
        auto ptr = std::make_shared<T>(data_queue.front());
        data_queue.pop();
        return ptr;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> uni_lock(mut);
        return data_queue.empty();
    }

};