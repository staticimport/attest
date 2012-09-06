
#include <attest/attest.h>

ATTEST(Checks, ATTEST_EQ)
{
  int x;
  ATTEST_EQ(1,1);
  x = 1;
  ATTEST_EQ(1,x);
  ATTEST_EQ(2,x);
}

