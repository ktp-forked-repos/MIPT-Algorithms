#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif

static const int NUMBER_HAZARD_POINTERS = 1000;
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

atomic<void *> &get_hazard_pointer(int index) {
    thread_local static hazard_pointer_helper helpers[2];
    return helpers[index].get();
}

// Ноды нумеруются в порядке добавления
// tail --- отдельный node
// head == node_1 -> ... -> node_n -> tail -> nullptr
template<class T>
class lock_free_queue {
public:
    struct Node {
        atomic<T *> t = {nullptr};
        atomic<Node *> next = {nullptr};
        Node *nextInToDeleteList = nullptr;
        atomic_int numberDeleted = {0};

        ~Node() {
            delete t;
        }
    };

    atomic<Node *> head = {new Node()};
    atomic<Node *> tail = {head.load()};
    atomic<Node *> toDelete = {nullptr};

    void enqueue(T item) {
        this_thread::yield();
        atomic<void *> &pointer = get_hazard_pointer(0);
        Node *node = new Node();
        T *t = new T(item);
//        dbgl("enqueue", node, t, head, tail);

        while (true) {
            Node *old_tail;
            do {
                old_tail = tail;
                pointer = old_tail;
            } while (tail != old_tail);

            T *old_t = nullptr;
            if (old_tail->t.compare_exchange_strong(old_t, t)) {
                old_tail->next = node;
                Node *curr_tail = tail;
                assert(curr_tail == old_tail);
                tail = node;
                return;
            }
        }
    }

    bool dequeue(T &item) {
        this_thread::yield();
//        dbgl("dequeue", head, tail);
        atomic<void *> &pointer = get_hazard_pointer(1);
        int attempts1 = 0;
        while (true) {
            ++attempts1;
            Node *old_head;
            int attempts2 = 0;
            do {
                old_head = head;
                pointer = old_head;
                ++attempts2;
            } while (head != old_head);

            if (old_head->next == nullptr) {
                return false;
            }
            if (head.compare_exchange_strong(old_head, old_head->next)) {
//                dbgl("\tdequeue", old_head, attempts1, attempts2);
                item = *old_head->t.exchange(nullptr);
                tryDelete(old_head);
                tryDeleteOthers();
                return true;
            }
        }
    }

    void deleteNode(Node *node) {
//        dbgl("\t\tdequeue", node);
//        delete node;
        assert(++node->numberDeleted == 1);
        node->next = nullptr;
        node->t = nullptr;
        node->nextInToDeleteList = nullptr;
    }

    void tryDelete(Node *node) {
        if (canDelete(node)) {
//            delete node;
            deleteNode(node);
        } else {
            while (!toDelete.compare_exchange_weak(node->nextInToDeleteList, node));
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
//        for (Node *temp = node; temp != nullptr; temp = temp->nextInToDeleteList) {
//            dbgl("\tdequeue", temp);
//        }
        while (node != nullptr) {
            Node *next = node->nextInToDeleteList;
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

int k = 2;
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
//    for (int i = 0; i < 10; ++i) {
//        thread([i] { dbg(i); }).join();
//    }
//    return 0;

    vector<thread> ts;

    for (int i = 0; i < consumers; ++i) {
        ts.emplace_back(consume);
    }
    for (int i = 0; i < produces; ++i) {
        ts.emplace_back(produce);
    }
//    for (int i = 0; i < k; ++i) {
//        ts.emplace_back(consume);
//        ts.emplace_back(produce);
//    }

    for (thread &t : ts) {
        t.join();
    }
    return 0;
}

#endif