#include <benchmark/benchmark.h>
#include <iostream>
#include "simdjson2.h"
#include <sstream>

using namespace simdjson2;
using namespace benchmark;
using namespace std;

const padded_string EMPTY_ARRAY("[]", 2);

static const char *TWITTER_JSON = SIMDJSON2_BENCHMARK_DATA_DIR "twitter.json";
static const char *NUMBERS_JSON = SIMDJSON2_BENCHMARK_DATA_DIR "numbers.json";

static void recover_one_string(State& state) {
  dom::parser parser;
  const std::string_view data = "\"one string\"";
  padded_string docdata{data};
  // we do not want mem. alloc. in the loop.
  auto error = parser.allocate(docdata.size());
  if(error) {
      cout << error << endl;
      return;
  }
  dom::element doc;
  if ((error = parser.parse(docdata).get(doc))) {
    cerr << "could not parse string" << error << endl;
    return;
  }
  for (simdjson2_unused auto _ : state) {
      std::string_view v;
      error = doc.get(v);
      if (error) {
        cerr << "could not get string" << error << endl;
        return;
      }
      benchmark::DoNotOptimize(v);
  }
}
BENCHMARK(recover_one_string);


static void serialize_twitter(State& state) {
  dom::parser parser;
  padded_string docdata;
  auto error = padded_string::load(TWITTER_JSON).get(docdata);
  if(error) {
      cerr << "could not parse twitter.json" << error << endl;
      return;
  }
  // we do not want mem. alloc. in the loop.
  if((error = parser.allocate(docdata.size()))) {
      cout << error << endl;
      return;
  }
  dom::element doc;
  if ((error = parser.parse(docdata).get(doc))) {
    cerr << "could not parse twitter.json" << error << endl;
    return;
  }
  size_t bytes = 0;
  for (simdjson2_unused auto _ : state) {
    std::string serial = simdjson2::minify(doc);
    bytes += serial.size();
    benchmark::DoNotOptimize(serial);
  }
  // we validate the result
  {
    auto serial = simdjson2::minify(doc);
    dom::element doc2; // we parse the minified output
    if ((error = parser.parse(serial).get(doc2))) { throw std::runtime_error("serialization error"); }
    auto serial2 = simdjson2::minify(doc2); // we minify a second time
    if(serial != serial2) { throw std::runtime_error("serialization mismatch"); }
  }
  // Gigabyte: https://en.wikipedia.org/wiki/Gigabyte
  state.counters["Gigabytes"] = benchmark::Counter(
	        double(bytes), benchmark::Counter::kIsRate,
	        benchmark::Counter::OneK::kIs1000); // For GiB : kIs1024
  state.counters["docs"] = Counter(double(state.iterations()), benchmark::Counter::kIsRate);
}
BENCHMARK(serialize_twitter)->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })->DisplayAggregatesOnly(true);


static void serialize_big_string_to_string(State& state) {
  dom::parser parser;
  std::vector<char> content;
  content.push_back('\"');
  for(size_t i = 0 ; i < 100000; i ++) {
    content.push_back('0' + char(i%10)); // we add what looks like a long list of digits
  }
  content.push_back('\"');
  dom::element doc;
  simdjson2::error_code error;
  if ((error = parser.parse(content.data(), content.size()).get(doc))) {
    cerr << "could not parse big string" << error << endl;
    return;
  }
  size_t bytes = 0;
  for (simdjson2_unused auto _ : state) {
    auto serial = simdjson2::to_string(doc);
    bytes += serial.size();
    benchmark::DoNotOptimize(serial);
  }
  // Gigabyte: https://en.wikipedia.org/wiki/Gigabyte
  state.counters["Gigabytes"] = benchmark::Counter(
	        double(bytes), benchmark::Counter::kIsRate,
	        benchmark::Counter::OneK::kIs1000); // For GiB : kIs1024
  state.counters["docs"] = Counter(double(state.iterations()), benchmark::Counter::kIsRate);
}
BENCHMARK(serialize_big_string_to_string)->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })->DisplayAggregatesOnly(true);


static void serialize_twitter_to_string(State& state) {
  dom::parser parser;
  padded_string docdata;
  auto error = padded_string::load(TWITTER_JSON).get(docdata);
  if(error) {
      cerr << "could not parse twitter.json" << error << endl;
      return;
  }
  // we do not want mem. alloc. in the loop.
  if((error = parser.allocate(docdata.size()))) {
      cout << error << endl;
      return;
  }
  dom::element doc;
  if ((error = parser.parse(docdata).get(doc))) {
    cerr << "could not parse twitter.json" << error << endl;
    return;
  }
  size_t bytes = 0;
  for (simdjson2_unused auto _ : state) {
    auto serial = simdjson2::to_string(doc);
    bytes += serial.size();
    benchmark::DoNotOptimize(serial);
  }
  // we validate the result
  {
    auto serial = simdjson2::to_string(doc);
    dom::element doc2; // we parse the stringify output
    if ((error = parser.parse(serial).get(doc2))) { throw std::runtime_error("serialization error"); }
    auto serial2 = simdjson2::to_string(doc2); // we stringify again
    if(serial != serial2) { throw std::runtime_error("serialization mismatch"); }
  }
  // Gigabyte: https://en.wikipedia.org/wiki/Gigabyte
  state.counters["Gigabytes"] = benchmark::Counter(
	        double(bytes), benchmark::Counter::kIsRate,
	        benchmark::Counter::OneK::kIs1000); // For GiB : kIs1024
  state.counters["docs"] = Counter(double(state.iterations()), benchmark::Counter::kIsRate);
}
BENCHMARK(serialize_twitter_to_string)->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })->DisplayAggregatesOnly(true);

static void serialize_twitter_string_builder(State& state) {
  dom::parser parser;
  padded_string docdata;
  auto error = padded_string::load(TWITTER_JSON).get(docdata);
  if(error) {
      cerr << "could not parse twitter.json" << error << endl;
      return;
  }
  // we do not want mem. alloc. in the loop.
  if((error = parser.allocate(docdata.size()))) {
      cout << error << endl;
      return;
  }
  dom::element doc;
  if ((error = parser.parse(docdata).get(doc))) {
    cerr << "could not parse twitter.json" << error << endl;
    return;
  }
  size_t bytes = 0;
  simdjson2::internal::string_builder<> sb;// not part of our public API, for internal use
  for (simdjson2_unused auto _ : state) {
    sb.clear();
    sb.append(doc);
    std::string_view serial = sb.str();
    bytes += serial.size();
    benchmark::DoNotOptimize(serial);
  }
  // Gigabyte: https://en.wikipedia.org/wiki/Gigabyte
  state.counters["Gigabytes"] = benchmark::Counter(
	        double(bytes), benchmark::Counter::kIsRate,
	        benchmark::Counter::OneK::kIs1000); // For GiB : kIs1024
  state.counters["docs"] = Counter(double(state.iterations()), benchmark::Counter::kIsRate);
}
BENCHMARK(serialize_twitter_string_builder)->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })->DisplayAggregatesOnly(true);


static void numbers_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr;
  simdjson2::error_code error;
  if ((error = parser.load(NUMBERS_JSON).get(arr))) {
    cerr << "could not read " << NUMBERS_JSON << " as an array: " << error << endl;
    return;
  }
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    for (auto e : arr) {
      double x;
      if ((error = e.get(x))) { cerr << "found a node that is not an number: " << error << endl; break;}
      container.push_back(x);
    }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_scan);

static void numbers_size_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr;
  simdjson2::error_code error;
  if ((error = parser.load(NUMBERS_JSON).get(arr))) {
    cerr << "could not read " << NUMBERS_JSON << " as an array: " << error << endl;
    return;
  }
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    container.resize(arr.size());
    size_t pos = 0;
    for (auto e : arr) {
      double x;
      if ((error = e.get(x))) { cerr << "found a node that is not an number: " << error << endl; break;}
      container[pos++] = x;
    }
    if(pos != container.size()) { cerr << "bad count" << endl; }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_size_scan);


static void numbers_type_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr;
  simdjson2::error_code error;
  if ((error = parser.load(NUMBERS_JSON).get(arr))) {
    cerr << "could not read " << NUMBERS_JSON << " as an array" << endl;
    return;
  }
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    for (auto e : arr) {
      dom::element_type actual_type = e.type();
      if(actual_type != dom::element_type::DOUBLE) {
        cerr << "found a node that is not an number?" << endl; break;
      }
      double x;
      error = e.get(x);
      container.push_back(x);
    }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_type_scan);

static void numbers_type_size_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr;
  simdjson2::error_code error;
  if ((error = parser.load(NUMBERS_JSON).get(arr))) {
    cerr << "could not read " << NUMBERS_JSON << " as an array: " << error << endl;
    return;
  }
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    container.resize(arr.size());
    size_t pos = 0;
    for (auto e : arr) {
      dom::element_type actual_type = e.type();
      if(actual_type != dom::element_type::DOUBLE) {
        cerr << "found a node that is not an number?" << endl; break;
      }
      double x;
      error = e.get(x);
      container[pos++] = x;
    }
    if(pos != container.size()) { cerr << "bad count" << endl; }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_type_size_scan);

static void numbers_load_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr;
  simdjson2::error_code error;
  for (simdjson2_unused auto _ : state) {
    // this may hit the disk, but probably just once
    if ((error = parser.load(NUMBERS_JSON).get(arr))) {
      cerr << "could not read " << NUMBERS_JSON << " as an array: " << error << endl;
      break;
    }
    std::vector<double> container;
    for (auto e : arr) {
      double x;
      if ((error = e.get(x))) { cerr << "found a node that is not an number: " << error << endl; break;}
      container.push_back(x);
    }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_load_scan);

static void numbers_load_size_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr;
  simdjson2::error_code error;
  for (simdjson2_unused auto _ : state) {
    // this may hit the disk, but probably just once
    if ((error = parser.load(NUMBERS_JSON).get(arr))) {
      cerr << "could not read " << NUMBERS_JSON << " as an array" << endl;
      break;
    }
    std::vector<double> container;
    container.resize(arr.size());
    size_t pos = 0;
    for (auto e : arr) {
      double x;
      if ((error = e.get(x))) { cerr << "found a node that is not an number?" << endl; break;}
      container[pos++] = x;
    }
    if(pos != container.size()) { cerr << "bad count" << endl; }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_load_size_scan);


#if SIMDJSON2_EXCEPTIONS


static void numbers_exceptions_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr = parser.load(NUMBERS_JSON);
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    for (double x : arr) {
      container.push_back(x);
    }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_exceptions_scan);

static void numbers_exceptions_size_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr = parser.load(NUMBERS_JSON);
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    container.resize(arr.size());
    size_t pos = 0;
    for (auto e : arr) {
      container[pos++] = double(e);
    }
    if(pos != container.size()) { cerr << "bad count" << endl; }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_exceptions_size_scan);



static void numbers_type_exceptions_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr = parser.load(NUMBERS_JSON);
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    for (auto e : arr) {
      dom::element_type actual_type = e.type();
      if(actual_type != dom::element_type::DOUBLE) {
        cerr << "found a node that is not an number?" << endl; break;
      }
      container.push_back(double(e));
    }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_type_exceptions_scan);

static void numbers_type_exceptions_size_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::array arr = parser.load(NUMBERS_JSON);
  for (simdjson2_unused auto _ : state) {
    std::vector<double> container;
    container.resize(arr.size());
    size_t pos = 0;
    for (auto e : arr) {
      dom::element_type actual_type = e.type();
      if(actual_type != dom::element_type::DOUBLE) {
        cerr << "found a node that is not an number?" << endl; break;
      }
      container[pos++] = double(e);
    }
    if(pos != container.size()) { cerr << "bad count" << endl; }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_type_exceptions_size_scan);

static void numbers_exceptions_load_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  for (simdjson2_unused auto _ : state) {
    // this may hit the disk, but probably just once
    dom::array arr = parser.load(NUMBERS_JSON);
    std::vector<double> container;
    for (double x : arr) {
      container.push_back(x);
    }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_exceptions_load_scan);

static void numbers_exceptions_load_size_scan(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  for (simdjson2_unused auto _ : state) {
    // this may hit the disk, but probably just once
    dom::array arr = parser.load(NUMBERS_JSON);
    std::vector<double> container;
    container.resize(arr.size());
    size_t pos = 0;
    for (double x : arr) {
      container[pos++] = x;
    }
    if(pos != container.size()) { cerr << "bad count" << endl; }
    benchmark::DoNotOptimize(container.data());
    benchmark::ClobberMemory();
  }
}
BENCHMARK(numbers_exceptions_load_size_scan);


static void twitter_count(State& state) {
  // Prints the number of results in twitter.json
  dom::parser parser;
  dom::element doc = parser.load(TWITTER_JSON);
  for (simdjson2_unused auto _ : state) {
    uint64_t result_count = doc["search_metadata"]["count"];
    if (result_count != 100) { return; }
  }
}
BENCHMARK(twitter_count);

static void twitter_default_profile(State& state) {
  // Count unique users with a default profile.
  dom::parser parser;
  dom::element doc = parser.load(TWITTER_JSON);
  for (simdjson2_unused auto _ : state) {
    set<string_view> default_users;
    for (dom::object tweet : doc["statuses"]) {
      dom::object user = tweet["user"];
      if (user["default_profile"]) {
        default_users.emplace(user["screen_name"]);
      }
    }
    if (default_users.size() != 86) { return; }
  }
}
BENCHMARK(twitter_default_profile);

static void twitter_image_sizes(State& state) {
  // Count unique image sizes
  dom::parser parser;
  dom::element doc = parser.load(TWITTER_JSON);
  simdjson2::error_code error;
  for (simdjson2_unused auto _ : state) {
    set<tuple<uint64_t, uint64_t>> image_sizes;
    for (dom::object tweet : doc["statuses"]) {
      dom::array media;
      if (! (error = tweet["entities"]["media"].get(media))) {
        for (dom::object image : media) {
          for (auto size : image["sizes"].get_object()) {
             image_sizes.emplace(size.value["w"], size.value["h"]);
          }
        }
      }
    }
    if (image_sizes.size() != 15) { return; };
  }
}
BENCHMARK(twitter_image_sizes);

#endif // SIMDJSON2_EXCEPTIONS

static void error_code_twitter_count(State& state) noexcept {
  // Prints the number of results in twitter.json
  dom::parser parser;
  simdjson2::error_code error;
  dom::element doc;
  if ((error = parser.load(TWITTER_JSON).get(doc))) { return; }
  for (simdjson2_unused auto _ : state) {
    uint64_t value;
    if ((error = doc["search_metadata"]["count"].get(value))) { return; }
    if (value != 100) { return; }
  }
}
BENCHMARK(error_code_twitter_count);

static void error_code_twitter_default_profile(State& state) noexcept {
  // Count unique users with a default profile.
  dom::parser parser;
  simdjson2::error_code error;
  dom::element doc;
  if ((error = parser.load(TWITTER_JSON).get(doc))) { std::cerr << error << std::endl; return; }
  for (simdjson2_unused auto _ : state) {
    set<string_view> default_users;

    dom::array tweets;
    if ((error = doc["statuses"].get(tweets))) { return; }
    for (dom::element tweet : tweets) {
      dom::object user;
      if ((error = tweet["user"].get(user))) { return; }
      bool default_profile{};
      if ((error = user["default_profile"].get(default_profile))) { return; }
      if (default_profile) {
        std::string_view screen_name;
        if ((error = user["screen_name"].get(screen_name))) { return; }
        default_users.insert(screen_name);
      }
    }

    if (default_users.size() != 86) { return; }
  }
}
BENCHMARK(error_code_twitter_default_profile);

static void error_code_twitter_image_sizes(State& state) noexcept {
  // Count unique image sizes
  dom::parser parser;
  simdjson2::error_code error;
  dom::element doc;
  if ((error = parser.load(TWITTER_JSON).get(doc))) { std::cerr << error << std::endl; return; }
  for (simdjson2_unused auto _ : state) {
    set<tuple<uint64_t, uint64_t>> image_sizes;
    dom::array statuses;
    if ((error = doc["statuses"].get(statuses))) { return; }
    for (dom::element tweet : statuses) {
      dom::array images;
      if (! (error = tweet["entities"]["media"].get(images))) {
        for (dom::element image : images) {
          dom::object sizes;
          if ((error = image["sizes"].get(sizes))) { return; }
          for (auto size : sizes) {
            uint64_t width, height;
            if ((error = size.value["w"].get(width))) { return; }
            if ((error = size.value["h"].get(height))) { return; }
            image_sizes.emplace(width, height);
          }
        }
      }
    }
    if (image_sizes.size() != 15) { return; };
  }
}
BENCHMARK(error_code_twitter_image_sizes);

static void parse_surrogate_pairs(State& state) {
  // NOTE: This mostly exists to show there's a tiny benefit to
  // loading and comparing both bytes of "\\u" simultaneously.
  // (which should also reduce the compiled code size).
  // The repeated surrogate pairs make this easier to measure.
  dom::parser parser;
  const std::string_view data = "\"\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\\uD834\\uDD1E\"";
  padded_string docdata{data};
  // we do not want mem. alloc. in the loop.
  auto error = parser.allocate(docdata.size());
  if (error) {
    cout << error << endl;
    return;
  }
  for (simdjson2_unused auto _ : state) {
    dom::element doc;
    if ((error = parser.parse(docdata).get(doc))) {
      cerr << "could not parse string" << error << endl;
      return;
    }
  }
}
BENCHMARK(parse_surrogate_pairs);

BENCHMARK_MAIN();
