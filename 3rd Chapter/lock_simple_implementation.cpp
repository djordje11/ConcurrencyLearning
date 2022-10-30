// simple impelemnation with one mutex for all calls on lock
#include <mutex>
#include <iostream>
static std::mutex m_lock;
template<class Lockable1, class Lockable2, class... LockableN>
void lock(Lockable1& lock1, Lockable2& lock2, LockableN&... lockn)
{
    std::cout << "Entering my lock" << std::endl;
    std::lock_guard<std::mutex> guard(m_lock);
    lock1.lock();
    lock2.lock();
    for(auto& it : {lockn...})
    {
        it.lock();
    }
}


int main()
{
    std::mutex m1, m2;
    ::lock(m1, m2);
}