#include <bits/stdc++.h>
using namespace std;
#ifdef LOCAL
#include "/home/dima/C++/debug.h"
#endif

// nullptr <- node_1 <- ... <- node_n == head
template<class T>
class lock_free_stack {
public:
    struct Node {
        Node *next;
        T t;

        Node(Node *next, T t) : next(next), t(t) {}
    };

    void push(T item) {
        Node *node = new Node(head, item);
        while (!head.compare_exchange_weak(node->next, node));
    }

    bool pop(T &item) {
//        if (!head)
//            return false;
//        Node *node = head;
//        head = node->next;
//        return node->t;

        Node *node = head;
        while (node != nullptr && head.compare_exchange_weak(node, node->next));
        if (node == nullptr) {
            return false;
        } else {
            item = node->t;
            return true;
        }
    }

    atomic<Node *> head = {nullptr};
};

// Ноды нумеруются в порядке добавления
// tail --- отдельный node
// head == node_1 -> ... -> node_n -> tail -> nullptr
template<class T>
class lock_free_queue {
public:
    struct Node {
        atomic<T *> t = {nullptr};
        atomic<Node *> next = {nullptr};
    };

    atomic<Node *> head = {new Node()};
    atomic<Node *> tail = {head.load()};

    void enqueue(T item) {
        Node *node = new Node();
        T *t = new T(item);
        Node *old_tail = tail;
        T *old_t = nullptr;
//        dbg(this_thread::get_id(), node, old_tail, old_tail->t.load());
        while (!old_tail->t.compare_exchange_weak(old_t, t)) {
            old_tail = tail;
            old_t = nullptr;
        }
//        dbgt(this_thread::get_id(), node, old_tail, old_tail->t.load());
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
};

#ifdef LOCAL

int produces = 4;
int consumers = 4;
int n = 100000;
lock_free_queue<int> q;

void produce() {
    for (int i = 0; i < n; ++i) {
        q.enqueue(1);
    }
}

void consume() {
    int x;
    for (int i = 0; i < n; ++i) {
        q.enqueue(x);
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