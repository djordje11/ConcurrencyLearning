#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>

std::mutex m;

std::unique_lock<std::mutex> f()
{
    m.lock();
    std::unique_lock<std::mutex> l(m, std::adopt_lock);
    return l;
}
void g()
{
    std::lock_guard<std::mutex> l(m);
    std::cout << "I managed to lock mutex" << std::endl;
}

int main()
{
    std::unique_lock<std::mutex> l1(f());
    std::thread t(g);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Messege from g will be called after this, it cannot be called until we unlock l1" << std::endl;
    l1.unlock();
    t.join();
    return 0;
}