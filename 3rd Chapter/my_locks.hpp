#include <mutex>
#include <string>

template<typename Mutex>
class my_lock_guard
{
    const Mutex& m_mutex;
public:
    explicit my_lock_guard(Mutex& mutex) : m_mutex(mutex)
    {
        m_mutex.lock();
    }
    my_lock_guard(Mutex& mutex, std::adopt_lock_t t) : m_mutex(mutex)
    {
    }
    ~my_lock_guard()
    {
        m_mutex.unlock();
    }
};


template<typename mutex_type>
class my_unique_lock
{
    mutex_type* m_mutex = nullptr;
    bool m_acquired_lock = false;
public:
    my_unique_lock() noexcept{}

    my_unique_lock(my_unique_lock&& other) noexcept
    {
        m_mutex = other.m_mutex;
        m_acquired_lock = other.m_acquired_lock;
        other.m_mutex = nullptr;
    }
    
    explicit my_unique_lock( mutex_type& m ) : m_mutex(&m)
    {
        m_mutex->lock();   
        m_acquired_lock = true;
    }

    my_unique_lock( mutex_type& m, std::defer_lock_t t ) : m_mutex(&m) {}

    my_unique_lock( mutex_type& m, std::try_to_lock_t t ) : m_mutex(&m)
    {
        if(m_mutex->try_lock())
            m_acquired_lock = true;
    }

    my_unique_lock( mutex_type& m, std::adopt_lock_t t ) : m_mutex(&m), m_acquired_lock(true) {}

    template< class Rep, class Period >
    my_unique_lock( mutex_type& m,
             const std::chrono::duration<Rep,Period>& timeout_duration ) 
    {
        if(m.try_lock_for(timeout_duration))
        {
            m_mutex = &m;
            m_acquired_lock = true;
        }
    }

    template< class Clock, class Duration >
    my_unique_lock( mutex_type& m,
             const std::chrono::time_point<Clock,Duration>& timeout_time )
    {
        if(m.try_lock_until(timeout_time))
        {
            m_mutex = &m;
            m_acquired_lock = true;
        }
    }

    ~my_unique_lock()
    {
        if(m_acquired_lock)
        {
            m_mutex->unlock();
        }
    }

    my_unique_lock& operator=(my_unique_lock&& other)
    {
        if(m_acquired_lock)
        {
            m_mutex->unlock();
        }
        m_mutex = &other.m_mutex;
        m_acquired_lock = other.m_acquired_lock;
        return *this;
    }

    mutex_type* mutex() const noexcept
    {
        return m_mutex;
    }

    void lock()
    {
        mutex()->lock();
    }

    bool try_lock()
    {
        if(!m_mutex)
            throw std::system_error(std::errc::operation_not_permitted);
        if(m_acquired_lock)
            throw std::system_error(std::errc::resource_deadlock_would_occur);
        return m_mutex->try_lock();
    }
};