#include "simdjson2.h"
#include "test_ondemand.h"

using namespace simdjson2;

namespace array_error_tests {
  using namespace std;

  template<typename V, typename T>
  bool assert_iterate(T array, V *expected, size_t N, simdjson2::error_code *expected_error, size_t N2) {
    /**
     * We use printouts because the assert_iterate is abstract and hard to
     * understand intuitively.
     */
    std::cout << "     --- assert_iterate ";
    size_t count = 0;
    for (auto elem : std::forward<T>(array)) {
      std::cout << "-"; std::cout.flush();
      V actual{};
      auto actual_error = elem.get(actual);
      if (count >= N) {
        if (count >= (N+N2)) {
          std::cerr << "FAIL: Extra error reported: " << actual_error << std::endl;
          return false;
        }
        std::cout << "[ expect: " << expected_error[count - N] << " ]"; std::cout.flush();
        ASSERT_ERROR(actual_error, expected_error[count - N]);
      } else {
        std::cout << "[ expect: SUCCESS ]"; std::cout.flush();
        ASSERT_SUCCESS(actual_error);
        std::cout << "{ expect value : "<< expected[count] << " }"; std::cout.flush();

        ASSERT_EQUAL(actual, expected[count]);
      }
      count++;
    }
    ASSERT_EQUAL(count, N+N2);
    std::cout << std::endl;
    return true;
  }

  template<typename V, size_t N, size_t N2, typename T>
  bool assert_iterate(T &array, V (&&expected)[N], simdjson2::error_code (&&expected_error)[N2]) {
    return assert_iterate<V, T&>(array, expected, N, expected_error, N2);
  }

  template<size_t N2, typename T>
  bool assert_iterate(T &array, simdjson2::error_code (&&expected_error)[N2]) {
    return assert_iterate<int64_t, T&>(array, nullptr, 0, expected_error, N2);
  }

  template<typename V, size_t N, typename T>
  bool assert_iterate(T &array, V (&&expected)[N]) {
    return assert_iterate<V, T&&>(array, expected, N, nullptr, 0);
  }

  template<typename V, size_t N, size_t N2, typename T>
  bool assert_iterate(T &&array, V (&&expected)[N], simdjson2::error_code (&&expected_error)[N2]) {
    return assert_iterate<V, T&&>(std::forward<T>(array), expected, N, expected_error, N2);
  }

  template<size_t N2, typename T>
  bool assert_iterate(T &&array, simdjson2::error_code (&&expected_error)[N2]) {
    return assert_iterate<int64_t, T&&>(std::forward<T>(array), nullptr, 0, expected_error, N2);
  }

  template<typename V, size_t N, typename T>
  bool assert_iterate(T &&array, V (&&expected)[N]) {
    return assert_iterate<V, T&&>(std::forward<T>(array), expected, N, nullptr, 0);
  }

  bool top_level_array_iterate_error() {
    TEST_START();
    ONDEMAND_SUBTEST("missing comma", "[1 1]",  assert_iterate(doc, { int64_t(1) }, { TAPE_ERROR }));
    ONDEMAND_SUBTEST("extra comma  ", "[1,,1]", assert_iterate(doc, { int64_t(1) }, { INCORRECT_TYPE, TAPE_ERROR }));
    ONDEMAND_SUBTEST("extra comma  ", "[,]",    assert_iterate(doc,                 { INCORRECT_TYPE, TAPE_ERROR }));
    ONDEMAND_SUBTEST("extra comma  ", "[,,]",   assert_iterate(doc,                 { INCORRECT_TYPE, TAPE_ERROR }));
    TEST_SUCCEED();
  }
  bool top_level_array_iterate_unclosed_error() {
    TEST_START();
    ONDEMAND_SUBTEST("unclosed extra comma", "[,",  assert_iterate(doc, { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed     ", "[1 ",        assert_iterate(doc, { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed extra comma", "[,,", assert_iterate(doc, { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed     ", "[1,",        assert_iterate(doc, { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed     ", "[1",         assert_iterate(doc, { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed     ", "[",          assert_iterate(doc, { INCOMPLETE_ARRAY_OR_OBJECT }));
    TEST_SUCCEED();
  }

  bool array_iterate_error() {
    TEST_START();
    ONDEMAND_SUBTEST("missing comma", R"({ "a": [1 1] })",  assert_iterate(doc["a"], { int64_t(1) }, { TAPE_ERROR }));
    ONDEMAND_SUBTEST("extra comma  ", R"({ "a": [1,,1] })", assert_iterate(doc["a"], { int64_t(1) }, { INCORRECT_TYPE, TAPE_ERROR }));
    ONDEMAND_SUBTEST("extra comma  ", R"({ "a": [1,,] })",  assert_iterate(doc["a"], { int64_t(1) }, { INCORRECT_TYPE, TAPE_ERROR }));
    ONDEMAND_SUBTEST("extra comma  ", R"({ "a": [,] })",    assert_iterate(doc["a"],                 { INCORRECT_TYPE, TAPE_ERROR}));
    ONDEMAND_SUBTEST("extra comma  ", R"({ "a": [,,] })",   assert_iterate(doc["a"],                 { INCORRECT_TYPE, TAPE_ERROR }));
    TEST_SUCCEED();
  }
  bool array_iterate_unclosed_error() {
    TEST_START();
    ONDEMAND_SUBTEST("unclosed extra comma", R"({ "a": [,)",  assert_iterate(doc["a"],                 { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed extra comma", R"({ "a": [,,)", assert_iterate(doc["a"],                 { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed     ", R"({ "a": [1 )",        assert_iterate(doc["a"],                 { INCOMPLETE_ARRAY_OR_OBJECT }));
    // TODO These pass the user values that may run past the end of the buffer if they are not careful
    // In particular, if the padding is decorated with the wrong values, we could cause overrun!
    ONDEMAND_SUBTEST("unclosed     ", R"({ "a": [1,)",        assert_iterate(doc["a"],                 { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed     ", R"({ "a": [1)",         assert_iterate(doc["a"],                 { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed     ", R"({ "a": [)",          assert_iterate(doc["a"],                 { INCOMPLETE_ARRAY_OR_OBJECT }));
    TEST_SUCCEED();
  }
  bool array_iterate_incomplete_error() {
    TEST_START();
#if SIMDJSON2_CHECK_EOF
    ONDEMAND_SUBTEST("unclosed after array", R"([ [1] )", assert_iterate(doc.get_array().at(0), { int64_t(1) }, { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed after array", R"([ [1,])", assert_iterate(doc.get_array().at(0), { int64_t(1) }, { INCORRECT_TYPE, TAPE_ERROR }));
    ONDEMAND_SUBTEST("unclosed after array", R"([ [1])",  assert_iterate(doc.get_array().at(0), { int64_t(1) }, { INCOMPLETE_ARRAY_OR_OBJECT }));
    ONDEMAND_SUBTEST("unclosed after array", R"([ [])",   assert_iterate(doc.get_array().at(0),                 { INCOMPLETE_ARRAY_OR_OBJECT }));
#else
    ONDEMAND_SUBTEST("unclosed after array", R"([ [1] )", assert_iterate(doc.get_array().at(0), { int64_t(1) }));
    ONDEMAND_SUBTEST("unclosed after array", R"([ [1,])", assert_iterate(doc.get_array().at(0), { int64_t(1) }, { INCORRECT_TYPE, TAPE_ERROR }));
    ONDEMAND_SUBTEST("unclosed after array", R"([ [1])",  assert_iterate(doc.get_array().at(0), { int64_t(1) }));
#endif
    TEST_SUCCEED();
  }

#ifdef SIMDJSON2_DEVELOPMENT_CHECKS
  bool out_of_order_array_iteration_error() {
    TEST_START();
    auto json = R"([ [ 1, 2 ] ])"_padded;
    SUBTEST("simdjson2_result<value>", test_ondemand_doc(json, [&](auto doc) {
      for (auto arr : doc) {
        for (auto subelement : arr) { ASSERT_SUCCESS(subelement); }
        ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
      }
      return true;
    }));
    SUBTEST("value", test_ondemand_doc(json, [&](auto doc) {
      for (auto element : doc) {
        ondemand::value arr;
        ASSERT_SUCCESS( element.get(arr) );
        for (auto subelement : arr) { ASSERT_SUCCESS(subelement); }
        ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
      }
      return true;
    }));
    SUBTEST("simdjson2_result<array>", test_ondemand_doc(json, [&](auto doc) {
      for (auto element : doc) {
        auto arr = element.get_array();
        for (auto subelement : arr) { ASSERT_SUCCESS(subelement); }
        ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
      }
      return true;
    }));
    SUBTEST("array", test_ondemand_doc(json, [&](auto doc) {
      for (auto element : doc) {
        ondemand::array arr;
        ASSERT_SUCCESS( element.get(arr) );
        for (auto subelement : arr) { ASSERT_SUCCESS(subelement); }
        ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
      }
      return true;
    }));
    TEST_SUCCEED();
  }

  bool out_of_order_top_level_array_iteration_error() {
    TEST_START();
    auto json = R"([ 1, 2 ])"_padded;
    SUBTEST("simdjson2_result<document>", test_ondemand_doc(json, [&](auto arr) {
      for (auto element : arr) { ASSERT_SUCCESS(element); }
      ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
      return true;
    }));
    SUBTEST("document", test_ondemand_doc(json, [&](auto doc) {
      ondemand::document arr;
      ASSERT_SUCCESS( std::move(doc).get(arr) );
      for (auto element : arr) { ASSERT_SUCCESS(element); }
      ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
      return true;
    }));
    SUBTEST("simdjson2_result<array>", test_ondemand_doc(json, [&](auto doc) {
      simdjson2_result<ondemand::array> arr = doc.get_array();
      for (auto element : arr) { ASSERT_SUCCESS(element); }
      ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
     return true;
    }));
    SUBTEST("array", test_ondemand_doc(json, [&](auto doc) {
      ondemand::array arr;
      ASSERT_SUCCESS( doc.get(arr) );
      for (auto element : arr) { ASSERT_SUCCESS(element); }
      ASSERT_ITERATE_ERROR( arr, OUT_OF_ORDER_ITERATION );
      return true;
    }));
    TEST_SUCCEED();
  }
#endif // SIMDJSON2_DEVELOPMENT_CHECKS

  bool run() {
    return
           top_level_array_iterate_error() &&
           top_level_array_iterate_unclosed_error() &&
           array_iterate_error() &&
           array_iterate_unclosed_error() &&
           array_iterate_incomplete_error() &&
#ifdef SIMDJSON2_DEVELOPMENT_CHECKS
           out_of_order_array_iteration_error() &&
           out_of_order_top_level_array_iteration_error() &&
#endif
           true;
  }
}

int main(int argc, char *argv[]) {
  return test_main(argc, argv, array_error_tests::run);
}
