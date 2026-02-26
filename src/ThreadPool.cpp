#include <iostream>
#include "ThreadPool.hpp"

using namespace std;


int main() {
    ThreadPool th(4);
    atomic<int>c(0);
    for(size_t i = 0; i < 100'000'000'000; ++i) {
        th.submit([&]{c.fetch_add(1,memory_order_relaxed);});
    }

    cout << "c = " << c.load() << endl;



    return 0;
}