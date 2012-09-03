
#include <attest/attest_checks.h>

#include <stdlib.h>
#include <string.h>

attest_test_error_set_t ATTEST_CURRENT_TEST_ERROR_SET;
static attest_test_error_t* ATTEST_TEST_ERROR_FREE_LIST = NULL;

static int ATTEST_MAX_ERRORS_PER_TEST = 10;

attest_test_error_t* attest_add_test_error()
{
  attest_test_error_t* error;
  if (ATTEST_CURRENT_TEST_ERROR_SET.error_count == ATTEST_MAX_ERRORS_PER_TEST)
    return 0;
  if (ATTEST_TEST_ERROR_FREE_LIST) {
    error = ATTEST_TEST_ERROR_FREE_LIST;
    ATTEST_TEST_ERROR_FREE_LIST = ATTEST_TEST_ERROR_FREE_LIST->next_error;
  } else {
    error = (attest_test_error_t*)malloc(sizeof(attest_test_error_t));
  }
  memset(error, 0, sizeof(attest_test_error_t));
  if (ATTEST_CURRENT_TEST_ERROR_SET.error_list_front)
    ATTEST_CURRENT_TEST_ERROR_SET.error_list_back->next_error = error;
  else
    ATTEST_CURRENT_TEST_ERROR_SET.error_list_front = error;
  ATTEST_CURRENT_TEST_ERROR_SET.error_list_back = error;
  ++ATTEST_CURRENT_TEST_ERROR_SET.error_count;
  return error;
}

void attest_clear_test_errors()
{
  ATTEST_CURRENT_TEST_ERROR_SET.error_count = 0;
  if (ATTEST_CURRENT_TEST_ERROR_SET.error_list_front) {
    ATTEST_CURRENT_TEST_ERROR_SET.error_list_back->next_error = ATTEST_TEST_ERROR_FREE_LIST;
    ATTEST_TEST_ERROR_FREE_LIST = ATTEST_CURRENT_TEST_ERROR_SET.error_list_front;
  }
}
