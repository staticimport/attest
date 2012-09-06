#ifndef H_ATTEST_CHECKS_H
#define H_ATTEST_CHECKS_H

#include <stdio.h>

#define ATTEST_TEST_ERROR_TEXT_LIMIT 512

typedef struct attest_test_error {
  char text[ATTEST_TEST_ERROR_TEXT_LIMIT];
  struct attest_test_error* next_error;
} attest_test_error_t;

typedef struct attest_test_error_set {
  int error_count;
  attest_test_error_t* error_list_front;
  attest_test_error_t* error_list_back;
} attest_test_error_set_t;

extern attest_test_error_set_t ATTEST_CURRENT_TEST_ERROR_SET;

attest_test_error_t* attest_add_test_error();
void attest_clear_test_errors();

#define ATTEST_CHECK_(expected, actual, check_name, passed) \
        do { \
          if (!passed) { \
            attest_test_error_t* error = attest_add_test_error(); \
            if (error) { \
              snprintf(error->text, ATTEST_TEST_ERROR_TEXT_LIMIT, \
                       "FAIL: %s(expected=%s, actual=%s) at %s:%d", \
                       check_name, expected, actual, __FILE__, __LINE__); \
            } \
          } \
        } while(0)

#define ATTEST_EQ(expected, actual) \
        ATTEST_CHECK_(#expected, #actual, "ATTEST_EQ", ((expected) == (actual)))
#define ATTEST_NE(expected, actual) \
        ATTEST_CHECK_(#expected, #actual, "ATTEST_NE", ((expected) != (actual)))
#define ATTEST_LT(expected, actual) \
        ATTEST_CHECK_(#expected, #actual, "ATTEST_LT", ((expected) < (actual)))
#define ATTEST_LTE(expected, actual) \
        ATTEST_CHECK_(#expected, #actual, "ATTEST_LTE", ((expected) <= (actual)))
#define ATTEST_GT(expected, actual) \
        ATTEST_CHECK_(#expected, #actual, "ATTEST_GT", ((expected) > (actual)))
#define ATTEST_GTE(expected, actual) \
        ATTEST_CHECK_(#expected, #actual, "ATTEST_GTE", ((expected) >= (actual)))
#define ATTEST_TRUE(x) \
        ATTEST_CHECK_("1", #x, "ATTEST_GTE", (x))
#define ATTEST_FALSE(x) \
        ATTEST_CHECK_("0", #x, "ATTEST_GTE", (!(x)))


#endif /* H_ATTEST_CHECKS_H */

