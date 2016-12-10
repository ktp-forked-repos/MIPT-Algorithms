#include <bits/stdc++.h>

class recursive_mutex {
public:
    void lock() {
        std::unique_lock<std::mutex> lock(m);
        std::thread::id id = std::this_thread::get_id();
        if (owner_id != id) {
            cv.wait(lock, [&] { return number == 0; });
            owner_id = id;
        }
        ++number;
    }

    void unlock() {
        std::unique_lock<std::mutex> lock(m);
        if (number == 0) {
            throw std::runtime_error("Вы вызвали unlock() больше раз чем lock()");
        }
        --number;
        if (number == 0) {
            owner_id = {};
            cv.notify_all();
        }
    }
private:
    std::mutex m;
    std::condition_variable cv;
    std::thread::id owner_id;
    size_t number = 0;
};