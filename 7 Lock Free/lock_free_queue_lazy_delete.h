#include <atomic>
#include <cassert>

// Ноды нумеруются в порядке добавления
// tail --- отдельный node
// head == node_1 -> ... -> node_n -> tail -> nullptr
template<class T>
class lock_free_queue {
public:
    struct Node {
        std::atomic<T *> t = {nullptr};
        std::atomic<Node *> next = {nullptr};

        ~Node() {
            delete t;
        }
    };

    std::atomic<Node *> head = {new Node()};
    std::atomic<Node *> tail = {head.load()};

    std::atomic<Node *> toDeleteBegin = {head.load()};
    std::atomic<Node *> toDeleteEnd = {head.load()};
    std::atomic_int threadsInDequeue = {0};

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
        ++threadsInDequeue;
        Node *old_head = head;
        bool success;
        while ((success = (old_head->next != nullptr)) && !head.compare_exchange_weak(old_head, old_head->next));
        if (!success) {
            return false;
        }
        item = *old_head->t;
        tryDelete(old_head);
        --threadsInDequeue;
        return true;
    }

    void tryDelete(Node *node) {
        if (threadsInDequeue == 1) {
            Node *toDeleteEnd = this->toDeleteEnd;
            Node *toDeleteBegin = this->toDeleteBegin.exchange(toDeleteEnd);
            if (threadsInDequeue == 1) {
                deleteNodes(toDeleteBegin, toDeleteEnd);
            } else {
                this->toDeleteBegin.exchange(toDeleteBegin);
            }
        } else {
            toDeleteEnd = node;
        }
    }

    void deleteNodes(Node *toDeleteBegin, Node *toDeleteEnd) {
        while (toDeleteBegin != toDeleteEnd) {
            Node *next = toDeleteBegin->next;
            delete toDeleteBegin;
            toDeleteBegin = next;
        }
    }

    ~lock_free_queue() {
        T t;
        while (dequeue(t));
        assert(head == tail);
        deleteNodes(toDeleteBegin, tail);
        delete tail;
    }
};