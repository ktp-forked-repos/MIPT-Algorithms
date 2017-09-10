#include <cstdlib>
#include <cassert>

extern void *mtalloc(size_t size) {
    return malloc(size);
}

extern void mtfree(void *data) {
    return free(data);
}