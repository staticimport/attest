
#include <attest/attest_functions.h>

#include <stdio.h>
#include <string.h>

attest_test_info_t ATTEST_TEST_INFOS[ATTEST_MAX_FUNCTION_COUNT];
unsigned ATTEST_NEXT_TEST_INFO_INDEX = 0;

typedef struct attest_test_result {
  int error_code;
  char error_text[512];
} attest_test_result_t;

typedef struct attest_result_set {
  unsigned num_tests;
  unsigned num_failed;
} attest_result_set_t;

static void attest_run_test(attest_test_info_t* test, attest_test_result_t* result);

void attest_run()
{
  attest_test_info_t* test = ATTEST_TEST_INFOS;
  attest_test_info_t* test_end = ATTEST_TEST_INFOS + ATTEST_NEXT_TEST_INFO_INDEX;
  char const* current_suite = 0;
  attest_result_set_t total_results = { 0, 0 };
  attest_result_set_t current_suite_results = { 0, 0 };
  attest_test_result_t result;
  for(; test != test_end; ++test)
  {
    attest_run_test(test, &result);
    if (!current_suite || strcmp(test->suite_name, current_suite)) {
      if (current_suite) {
        printf("--> %s: %u tests, %u failed\n\n", current_suite,
               current_suite_results.num_tests, current_suite_results.num_failed);
      }
      current_suite = test->suite_name;
      memset(&current_suite_results, 0, sizeof(attest_result_set_t));
    }
    ++current_suite_results.num_tests;
    ++total_results.num_tests;
    if (result.error_code) {
      ++current_suite_results.num_failed;
      ++total_results.num_failed;
    }
  }
  if (current_suite) {
    printf("--> %s: %u tests, %u failed\n\n", current_suite,
        current_suite_results.num_tests, current_suite_results.num_failed);
  }
  printf("Overall: %u tests, %u failed\n", total_results.num_tests, total_results.num_failed);
}

void attest_run_test(attest_test_info_t* test, attest_test_result_t* result)
{
  printf("running %s...", test->test_name);
  fflush(stdout);
  test->test_function();
  printf("success\n");
  result->error_code = 0;
  result->error_text[0] = 0;
}

