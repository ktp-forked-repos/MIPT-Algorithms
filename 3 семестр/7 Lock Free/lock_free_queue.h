#include <bits/stdc++.h>

static const int NUMBER_HAZARD_POINTERS = 1000;
struct hazard_pointer {
    std::atomic<std::thread::id> id;
    std::atomic<void *> pointer = {nullptr};
};
std::array<hazard_pointer, NUMBER_HAZARD_POINTERS> hazard_pointers;

struct hazard_pointer_helper {
    hazard_pointer *pointer;

    hazard_pointer_helper() {
        std::thread::id id = std::this_thread::get_id();
        for (int i = 0; i < NUMBER_HAZARD_POINTERS; ++i) {
            std::thread::id empty_id;
            if (hazard_pointers[i].id.compare_exchange_weak(empty_id, id)) {
                pointer = &hazard_pointers[i];
                return;
            }
        }
        throw std::runtime_error("No more hazard pointers available");
    }

    std::atomic<void *> &get() {
        return pointer->pointer;
    }

    ~hazard_pointer_helper() {
        pointer->pointer = nullptr;
        pointer->id = std::thread::id();
    }
};

std::atomic<void *> &get_hazard_pointer(int index) {
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
        std::atomic<T *> data = {nullptr};
        std::atomic<Node *> next = {nullptr};
        Node *nextInToDeleteList = nullptr;
    };

    std::atomic<Node *> head = {new Node()};
    std::atomic<Node *> tail = {head.load()};
    std::atomic<Node *> nodesToDelete = {nullptr};

    void enqueue(T data) {
        std::this_thread::yield();
        std::atomic<void *> &pointer = get_hazard_pointer(0);
        Node *node = new Node();
        T *data_pointer = new T(data);
        while (true) {
            Node *old_tail;
            do {
                old_tail = tail;
                pointer = old_tail;
            } while (tail != old_tail);

            T *old_t = nullptr;
            if (old_tail->data.compare_exchange_strong(old_t, data_pointer)) {
                old_tail->next = node;
                tail = node;
                return;
            }
        }
    }

    bool dequeue(T &data) {
        std::atomic<void *> &pointer = get_hazard_pointer(1);
        while (true) {
            Node *old_head;
            do {
                old_head = head;
                pointer = old_head;
            } while (head != old_head);

            if (old_head->next == nullptr) {
                return false;
            }
            if (head.compare_exchange_strong(old_head, old_head->next)) {
                T *data_pointer = old_head->data.exchange(nullptr);
                data = *data_pointer;
                delete data_pointer;
                tryDelete(old_head);
                tryDeleteOthers();
                return true;
            }
        }
    }

    void tryDelete(Node *node) {
        if (canDelete(node)) {
            delete node;
        } else {
            while (!nodesToDelete.compare_exchange_weak(node->nextInToDeleteList, node));
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
        Node *node = nodesToDelete.exchange(nullptr);
        while (node != nullptr) {
            Node *next = node->nextInToDeleteList;
            tryDelete(node);
            node = next;
        }
    }

    ~lock_free_queue() {
        T data;
        while (dequeue(data));

        for (int i = 0; i < NUMBER_HAZARD_POINTERS; ++i) {
            hazard_pointers[i].pointer = nullptr;
            hazard_pointers[i].id = std::thread::id();
        }
        tryDeleteOthers();

        delete tail;
    }
};