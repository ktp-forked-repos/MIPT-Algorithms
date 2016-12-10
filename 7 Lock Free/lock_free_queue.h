#include <bits/stdc++.h>
using namespace std;

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
        atomic<T *> t;
        atomic<Node *> next;
    };

    atomic<Node *> head = {new Node()};
    atomic<Node *> tail = {head.load()};
    mutex m;

    void enqueue(T item) {
        lock_guard<mutex> lock(m);
        T *t = new T(item);
        Node *node = new Node();
        tail.load()->t = t;
        tail.load()->next = node;
        tail = node;
    }

    bool dequeue(T &item) {
        lock_guard<mutex> lock(m);
        if (head == tail) {
            return false;
        }
        item = *head.load()->t.load();
        head = head.load()->next;
        return true;
    }
};

#ifdef LOCAL

int main() {
    int x;
    lock_free_stack<int> s;
    lock_free_queue<int> q;

    q.enqueue(1);
    q.enqueue(2);

    assert(q.dequeue(x));
    assert(x == 1);

    assert(q.dequeue(x));
    assert(x == 2);

    assert(!q.dequeue(x));
    return 0;
}

#endif