#include <mutex>
#include <thread>
#include <iostream>


void do_something(){}
void f()
{
    std::mutex m;
    m.lock(); 
    do_something();


}

int main()
{
    std::mutex mutex;
    std::unique_lock<std::mutex> lck1(mutex);
    std::unique_lock<std::mutex> lck2(mutex, std::defer_lock);
    std::unique_lock<std::mutex> lck3(mutex, std::adopt_lock);

    std::cout << lck1.owns_lock() << std::endl;
    std::cout << lck2.owns_lock() << std::endl;
    std::cout << lck3.owns_lock() << std::endl;
    




    return 0;
}