#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
class Object
{
    std::vector<int> m_xs;
    std::mutex m_mutex;
public:
    Object(const std::vector<int>& xs) : m_xs(xs) {}
    friend void swap_with_deadlock(Object &r, Object &l);
    friend void swap_without_deadlock(Object &r, Object &l);
};

void swap_with_deadlock(Object &r, Object &l)
{
    std::lock_guard<std::mutex> lock_r(r.m_mutex);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // -> os can stop this thread here 

    std::lock_guard<std::mutex> lock_l(l.m_mutex);

    std::swap(r.m_xs, l.m_xs);
}

void swap_without_deadlock(Object &r, Object &l)
{
    std::lock(r.m_mutex, l.m_mutex);
    std::lock_guard<std::mutex> lock_r(r.m_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_l(l.m_mutex, std::adopt_lock);

    std::swap(r.m_xs, l.m_xs);
}

template<typename F>
void run(F f)
{
    Object l({1, 2, 3});
    Object r({7, 8, 9});

    std::thread t1(f, std::ref(l), std::ref(r));
    std::thread t2(f, std::ref(r), std::ref(l));

    t1.join();
    t2.join();
}


int main()
{
//  run to see deadlock 
//  run(swap_with_deadlock);
    run(swap_without_deadlock);
    return 0;
}









