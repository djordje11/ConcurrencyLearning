#include <vector>
#include <thread>
#include <iostream>
#include <chrono>

class object_with_long_initialization
{
    std::vector<int> vec;
public:
    object_with_long_initialization()
    {
        std::cout << "Pokrenuo inicijalizaciju u niti " << std::this_thread::get_id() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        vec = std::vector<int>(10000);
    }
};



void f()
{
    std::cout << "Usao u f sa niti: " << std::this_thread::get_id() << std::endl;
    static object_with_long_initialization obj;
    std::cout << "Pozivam f" << std::endl;
}

int main()
{
    std::cout << "main" << std::endl;
    std::thread t1(f);
    std::thread t2(f);

    t1.join();
    t2.join();
}