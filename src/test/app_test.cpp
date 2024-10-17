#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../app/server.h"

// ####################
// init
// ####################
Test(init, _1) {
  int expected = 10;
  int actual = add(8, 2);
  cr_assert_eq(actual, expected);
}
