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
        T t;
        Node *next;

        Node(T t, Node *next = nullptr) : t(t), next(next) {}
    };

    std::atomic<Node *> head = {nullptr};
    std::atomic<Node *> tail = {nullptr};
    std::mutex m;

    void enqueue(T item) {
        std::lock_guard<std::mutex> lock(m);
        Node *node = new Node(item);
        if (tail) {
            tail.load()->next = node;
            tail = node;
        } else {
            head = node;
            tail = node;
        }
    }

    bool dequeue(T &item) {
        std::lock_guard<std::mutex> lock(m);
        if (head == nullptr) {
            return false;
        }
        Node *node = head;
        head = node->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        item = node->t;
        delete node;
        return true;
    }
};

#ifdef LOCAL
int main() {
    lock_free_stack<int> s;
    lock_free_queue<int> q;
    return 0;
}
#endif