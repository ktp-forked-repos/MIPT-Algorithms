#include <bits/stdc++.h>

class account {
public:
    void deposit(unsigned int x) {
        sum.fetch_add(x);
        cv.notify_all();
    }

    void withdraw(unsigned int y) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] { return sum >= y; });
        sum -= y;
    }

private:
    std::atomic_size_t sum = {0};
    std::mutex m;
    std::condition_variable cv;
};