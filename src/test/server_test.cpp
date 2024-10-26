#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../app/server.hpp"

// ####################
// load_config
// ####################
Test(load_config, _1) {
  Config expected = {8080, ""};
  Config* actual = Config::load();
  cr_assert_eq(actual->PORT, expected.PORT);
  cr_assert_eq(actual->WWW_DIR, expected.WWW_DIR);

  delete actual;
}
