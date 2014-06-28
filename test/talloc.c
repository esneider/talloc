#include "test.h"
#include "talloc.h"

int main(void) {

    void *buffer;

    buffer = talloc(0, NULL);
    test(buffer, "alloc 0-sized buffer");
    buffer = tfree(buffer);

    return 0;
}

