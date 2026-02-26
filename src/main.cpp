#include "ThreadPool.hpp"
#include <iostream>
using namespace std;

int main()
{
    ThreadPool th(4);

    atomic<int> c{0};
    for (size_t i = 0; i < int(10000); ++i)
    {
        th.submit([&]
                  { c.fetch_add(1, memory_order_relaxed); });
    }
    th.shutdown();

    cout << "counter = " << c.load() << endl;

    return 0;
}