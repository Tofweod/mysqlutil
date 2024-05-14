#ifndef TOF_TEST_UTIL
#define TOF_TEST_UTIL

#include "log.h"

#define RUN_TEST(test_idx) test##test_idx()

#define TEST(test_idx)                                                         \
    do {                                                                       \
        INFO_PRINT(stdout, "processing test %d...\n", test_idx);               \
        RUN_TEST(test_idx);                                                    \
    } while (0);

#endif
