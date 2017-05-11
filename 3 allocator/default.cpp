#include <cstdlib>
#include <cassert>

extern void *mtalloc(size_t size) {
    assert(0);
    return malloc(size);
}

extern void mtfree(void *data) {
    return free(data);
}