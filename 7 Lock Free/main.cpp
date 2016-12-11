#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif

static const int NUMBER_HAZARD_POINTERS = 100;
struct hazard_pointer {
    atomic<thread::id> id;
    atomic<void *> pointer = {nullptr};
};
std::array<hazard_pointer, NUMBER_HAZARD_POINTERS> hazard_pointers;

struct hazard_pointer_helper {
    hazard_pointer *pointer;

    hazard_pointer_helper() {
        thread::id id = this_thread::get_id();
        for (int i = 0; i < NUMBER_HAZARD_POINTERS; ++i) {
            thread::id empty_id;
            if (hazard_pointers[i].id.compare_exchange_weak(empty_id, id)) {
                pointer = &hazard_pointers[i];
                return;
            }
        }
        throw runtime_error("No more hazard pointers available");
    }

    atomic<void *> &get() {
        return pointer->pointer;
    }

    ~hazard_pointer_helper() {
        pointer->pointer = nullptr;
        pointer->id = thread::id();
    }
};

atomic<void *> &get_hazard_pointer() {
    thread_local static hazard_pointer_helper helper;
    return helper.get();
}

// Ноды нумеруются в порядке добавления
// tail --- отдельный node
// head == node_1 -> ... -> node_n -> tail -> nullptr
template<class T>
class lock_free_queue {
public:
    struct Node {
        atomic<T *> t = {nullptr};
        Node *next = {nullptr};

        ~Node() {
            delete t;
        }
    };

    atomic<Node *> head = {new Node()};
    atomic<Node *> tail = {head.load()};
    atomic<Node *> toDelete = {nullptr};

    void enqueue(T item) {
        Node *node = new Node();
        T *t = new T(item);
        Node *old_tail = tail;
        T *old_t = nullptr;
        while (!old_tail->t.compare_exchange_weak(old_t, t)) {
            old_tail = tail;
            old_t = nullptr;
        }
        old_tail->next = node;
        Node *curr_tail = tail;
        assert(curr_tail == old_tail);
        tail = node;
    }

    bool dequeue(T &item) {
        Node *old_head = head;
        bool success;
        while ((success = (old_head->next != nullptr)) && !head.compare_exchange_weak(old_head, old_head->next));
        if (!success) {
            return false;
        }
        item = *old_head->t;
        return true;
    }

    bool dequeue2(T &item) {
//        atomic<void *> &pointer = get_hazard_pointer();
        while (true) {
//            Node *old_head;
//            do {
//                old_head = head;
//                pointer = old_head;
//            } while (head != old_head);
            Node *old_head = head;

            if (old_head->next == nullptr) {
                return false;
            }
            if (head.compare_exchange_strong(old_head, old_head->next)) {
                item = *old_head->t;
//                pointer = nullptr;
//                tryDelete(old_head);
//                tryDeleteOthers();
                return true;
            }
        }
    }

    void tryDelete(Node *node) {
        if (canDelete(node)) {
            delete node;
        } else {
            while (toDelete.compare_exchange_weak(node->next, node));
        }
    }

    bool canDelete(Node *node) {
        for (int i = 0; i < NUMBER_HAZARD_POINTERS; ++i) {
            if (hazard_pointers[i].pointer == node) {
                return false;
            }
        }
        return true;
    }

    void tryDeleteOthers() {
        Node *node = toDelete.exchange(nullptr);
        while (node != nullptr) {
            Node *next = node->next;
            tryDelete(node);
            node = next;
        }
    }

//    ~lock_free_queue() {
//        T t;
//        while (dequeue(t));
//        assert(head == tail);
//
//        for (int i = 0; i < NUMBER_HAZARD_POINTERS; ++i) {
//            hazard_pointers[i].pointer = nullptr;
//            hazard_pointers[i].id = thread::id();
//        }
//        tryDeleteOthers();
//
//        delete tail;
//    }
};

#ifdef LOCAL

int produces = 10;
int consumers = 10;
int n = 10000;
lock_free_queue<int> q;

void produce() {
    for (int i = 0; i < n; ++i) {
        q.enqueue(1);
    }
}

void consume() {
    int x;
    for (int i = 0; i < n; ++i) {
        q.dequeue(x);
    }
}

int main() {
    vector<thread> ts;
    for (int i = 0; i < produces; ++i) {
        ts.emplace_back(produce);
    }
    for (int i = 0; i < consumers; ++i) {
        ts.emplace_back(consume);
    }

    for (thread &t : ts) {
        t.join();
    }
    return 0;
}

#endif