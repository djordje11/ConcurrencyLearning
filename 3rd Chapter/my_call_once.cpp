#include <mutex>
#include <functional>
#include <utility>
#include <iostream>
class my_once_flag
{
    bool m_called = false;
    std::mutex m_mutex;
public:
    my_once_flag()
    {
    }
    my_once_flag(const my_once_flag&) = delete;
    my_once_flag& operator=(const my_once_flag&) = delete;    
    template< class Callable, class... Args >
    friend void my_call_once( my_once_flag& flag, Callable&& f, Args&&... args );
};


template< class Callable, class... Args >
void my_call_once( my_once_flag& flag, Callable&& f, Args&&... args )
{
    if(flag.m_called)
        return;

    std::lock_guard lg(flag.m_mutex);

    if(flag.m_called)
        return;

    std::invoke(std::forward<Callable>(f), std::forward(args)...);

    flag.m_called = true;

}

int main()
{
    auto lambda = [](){std::cout << "Write once" << std::endl;};
    my_once_flag fl;
    my_call_once(fl, lambda);

    my_call_once<void (*)()>;
    
    return 0;
}