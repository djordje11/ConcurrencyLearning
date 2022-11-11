#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <chrono>

class my_condition_variable
{
    pthread_cond_t cond;
public:
    my_condition_variable() {}
    void wait( std::unique_lock<std::mutex>& lock )
    {
        int res = 0;
        pthread_mutex_t* mutex = lock.mutex()->native_handle();
        res = pthread_cond_wait(&cond, mutex);

        if(res)
        {
            throw std::string("my_condition_variable::wait failed in pthread_cond_wait");
        }
    }

    template<typename Predicate>
    void wait( std::unique_lock<std::mutex>& lock, Predicate pred)
    {
        while(!pred())
        {
            wait(lock);
        }
    }

    void notify_one() noexcept
    {
        pthread_cond_signal(&cond);
    }

    void notify_all() noexcept
    {
        pthread_cond_broadcast(&cond);
    }

    inline bool do_wait_until(std::unique_lock<std::mutex>& lock, struct timespec *abstime)
    {
        int cond_res;
        pthread_mutex_t* m = lock.mutex()->native_handle();
        cond_res = pthread_cond_timedwait(&cond, m, abstime);

        if(cond_res == ETIMEDOUT)
            return false;

        if(cond_res)
            throw std::string("my_condition_variable::do_wait_until failed in pthread_cond_timedwait");
        
        return true;
    }
    //template< class Rep, class Period >
    std::cv_status wait_for( std::unique_lock<std::mutex>& lock, const std::chrono::duration<int, std::ratio<1, 100>>& rel_time)
    {
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(rel_time);
        auto nanosec = rel_time - sec;
        auto sec_since_epoh = time(NULL);
        timespec tm{sec_since_epoh + sec.count(), nanosec.count()};

        if(do_wait_until(lock, &tm))
        {
            return std::cv_status::no_timeout;
        }

        return std::cv_status::timeout;
    }
};
