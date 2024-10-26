#include <cstdlib>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../app/server.hpp"

// ####################
// Config_load
// ####################
Test(Config_load, from_env) {
  Config expected = {8080, "/srv/www"};
  Config* actual = Config::load();
  cr_assert_eq(actual->PORT, expected.PORT);
  cr_assert_eq(actual->WWW_DIR, expected.WWW_DIR);

  delete actual;
}

Test(Config_load, from_file) {
  using namespace std;

  string cwd = string(getenv("PWD"));
  Config expected = {8080, "/srv/www"};
  Config* actual = Config::load(cwd + "/src/assets/config.json");
  cr_assert_eq(actual->PORT, expected.PORT);
  cr_assert_eq(actual->WWW_DIR, expected.WWW_DIR);

  delete actual;
}
