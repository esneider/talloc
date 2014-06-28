#ifndef __TEST_H__
#define __TEST_H__

#include <stdio.h>
#include <assert.h>

#define test(test, format, ...)                        \
    do {                                               \
        printf("Test: " format "... ", ##__VA_ARGS__); \
        fflush(stdout);                                \
        assert(test);                                  \
        printf("PASSED\n");                            \
    } while (0)

#endif /* __TEST_H__ */

