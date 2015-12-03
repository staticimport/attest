
#include <attest/attest_checks.h>
#include <attest/attest_functions.h>

#include <stdio.h>
#include <string.h>

attest_test_info_t ATTEST_TEST_INFOS[ATTEST_MAX_FUNCTION_COUNT];
unsigned ATTEST_NEXT_TEST_INFO_INDEX = 0;

static int attest_run_test(attest_test_info_t* test);

void attest_all(void)
{
  attest_test_info_t* test = ATTEST_TEST_INFOS;
  attest_test_info_t* test_end = ATTEST_TEST_INFOS + ATTEST_NEXT_TEST_INFO_INDEX;
  char const* current_suite = 0;
  int current_suite_fails = 0;
  int total_fails = 0;
  int current_suite_tests = 0;
  int total_tests = 0;
  for(; test != test_end; ++test)
  {
    if (!current_suite || strcmp(test->suite_name, current_suite)) {
      if (current_suite) {
        printf("--> %s: %u tests, %u failed\n\n", current_suite, 
               current_suite_tests, current_suite_tests);
      }
      current_suite = test->suite_name;
      current_suite_fails = 0;
      current_suite_tests = 0;
    }
    if (attest_run_test(test)) {
      ++current_suite_fails;
      ++total_fails;
    }
    ++current_suite_tests;
    ++total_tests;
  }
  if (current_suite) {
    printf("--> %s: %u tests, %u failed\n\n", current_suite, 
          current_suite_tests, current_suite_fails);
  }
  printf("Overall: %u tests, %u failed\n", total_tests, total_fails);
}

int attest_run_test(attest_test_info_t* test)
{
  int test_counter;
  attest_test_error_t* test_error;

  attest_clear_test_errors();
  printf("running %s...", test->test_name);
  fflush(stdout);
  test->test_function();
  if (ATTEST_CURRENT_TEST_ERROR_SET.error_count) {
    printf("failed\n");
    test_counter = 0;
    test_error = ATTEST_CURRENT_TEST_ERROR_SET.error_list_front;
    while (test_error) {
      printf("  (%d) %s\n", test_counter++, test_error->text);
      test_error = test_error->next_error;
    }
    return -1;
  } else {
    printf("success\n");
    return 0;
  }
}

