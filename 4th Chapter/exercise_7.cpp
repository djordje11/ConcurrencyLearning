#include <future>
#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>

std::future<int> process_vector(std::vector<int> x)
{
    const unsigned thread_num = 4;
    unsigned chunk = x.size() / thread_num;
    if(chunk == 0) return std::async([&x](){return std::accumulate(x.begin(), x.end(), 0);});
    std::vector<std::future<int>> results;
    auto lambda = 
    [](auto begin, auto end)
    { 
        return std::accumulate(begin, end, 0); 
    };

    for (auto it = x.begin(), end = x.end(); it != end;)
    {
        auto next = std::distance(it, end) >= chunk ? it + chunk : end;
        results.push_back(
            std::async(lambda, it, next)); 
        it = next;
    }

    return std::async([all_results = std::move(results)]() mutable
        {
            std::vector<int> gathered_results;
            for(auto& it : all_results)
            {
                gathered_results.push_back(it.get());
            }
            return std::accumulate(gathered_results.begin(), gathered_results.end(), 0);
        }
    );
}

int main()
{
    auto f = process_vector({1, 2, 3, 4});//, 5, 6, 7, 8, 9, 10}); //, 11, 12, 13, 14, 15});

    std::cout << f.get() << std::endl;
    return 0;
}