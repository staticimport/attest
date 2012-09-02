#ifndef H_ATTEST_FUNCTIONS_H
#define H_ATTEST_FUNCTIONS_H

typedef struct attest_test_info {
  char const* suite_name;
  char const* test_name;
  void (*test_function)(void);
} attest_test_info_t;

#define ATTEST_MAX_FUNCTION_COUNT 2048
extern attest_test_info_t ATTEST_TEST_INFOS[ATTEST_MAX_FUNCTION_COUNT];
extern unsigned ATTEST_NEXT_TEST_INFO_INDEX;

#define ATTEST(suitename, testname) \
  static void attest_bootstrap_test_##suitename##_##testname() __attribute__((constructor)); \
  static void attest_test_##suitename##_##testname(); \
  void attest_bootstrap_test_##suitename##_##testname() { \
    attest_test_info_t* info = ATTEST_TEST_INFOS[ATTEST_NEXT_TEST_INFO_INDEX++]; \
    info->suite_name = #suitename; \
    info->test_name = #testname; \
    info->test_function = &attest_test_##suitename##_##testname; \
  } \
  void attest_test_##suitename##_##testname()

void attest_run();

#endif /* H_ATTEST_FUNCTIONS_H */

