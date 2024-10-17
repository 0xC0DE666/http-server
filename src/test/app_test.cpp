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

// ####################
// load_config
// ####################
Test(load_config, _1) {
  Config expected = {6969, "/www"};
  Config* actual = load_config("/home/damian/dojo/cpp/http-server/src/assets/config.json");
  cr_assert_eq(actual->port, expected.port);
  cr_assert_eq(actual->public_dir, expected.public_dir);

  delete actual;
}
