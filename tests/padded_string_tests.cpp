
#include "simdjson2.h"

#include <cstdlib>

// this test is needed, because memcpy may be invoked on a null pointer
// otherwise
static void testNullString() {
  std::string_view empty;
  simdjson2::padded_string blah(empty);
}

int main() {

  testNullString();
}
