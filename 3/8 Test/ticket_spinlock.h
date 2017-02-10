#include <bits/stdc++.h>

class ticket_spinlock {
public:
    void lock() {
        size_t thisThreadTicket = nextFreeTicket.fetch_add(1);
        while (ownerTicket.load() != thisThreadTicket) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        ownerTicket.store(ownerTicket.load() + 1);
    }

private:
    std::atomic<size_t> ownerTicket = {0};
    std::atomic<size_t> nextFreeTicket = {0};
};