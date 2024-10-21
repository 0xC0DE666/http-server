#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../app/server.hpp"

// ####################
// load_config
// ####################
Test(load_config, _1) {
  Config expected = {6969, "/www"};
  Config* actual = Config::load("/home/damian/dojo/cpp/http-server/src/assets/config.json");
  cr_assert_eq(actual->PORT, expected.PORT);
  cr_assert_eq(actual->PUBLIC_DIR, expected.PUBLIC_DIR);

  delete actual;
}
