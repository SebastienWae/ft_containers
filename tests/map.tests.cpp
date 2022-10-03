#include <cassert>
#include <string>
#include <vector>

#include "utils/chrono.hpp"
#include "utils/logger.hpp"
#include "utils/testing_struct.hpp"

#ifdef STD
#include <map>
#include <utility>
#define LIB std
#else
#include "../ft/map.hpp"
#include "../ft/utility.hpp"
#define LIB ft
#endif

#define MAP_PERF_BASE_SIZE 1000000 // 1 000 000

#define MAP_PRINT_STATE(map) map_print_state(map, #map)
template <typename K, typename V>
void map_print_state(LIB::map<K, V> m, std::string const &name) {
  print_data(name);
  print_data(m.size());
  for (typename LIB::map<K, V>::const_iterator it = m.begin(); it != m.end();
       ++it) {
    print_data(it->first);
    print_data(it->second);
  }
}

#define MAP_CALL_TEST_FN(fn, k, v) fn<k, v>(#k, #v)

template <typename K, typename V>
void map_impl_test(std::string const &key_type, std::string const &val_type) {
  print_data(key_type + ":" + val_type);

  std::vector<LIB::pair<K, V> > data;
  for (int i = 0; i < 10; ++i) {
    data.push_back(LIB::make_pair(i, i));
    ;
  }

  std::vector<LIB::pair<K, V> > data2;
  for (int i = 10; i < 20; ++i) {
    data2.push_back(LIB::make_pair(i, i));
    ;
  }

  LIB::map<K, V> default_constructor;
  MAP_PRINT_STATE(default_constructor);

  LIB::map<K, V> range_constructor(data.rbegin(), data.rend());
  MAP_PRINT_STATE(range_constructor);

  LIB::map<K, V> copy_constructor(range_constructor);
  MAP_PRINT_STATE(copy_constructor);

  LIB::map<K, V> const_default_constructor;
  MAP_PRINT_STATE(const_default_constructor);

  LIB::map<K, V> const_range_constructor(data.rbegin(), data.rend());
  MAP_PRINT_STATE(const_range_constructor);

  LIB::map<K, V> const_copy_constructor(const_range_constructor);
  MAP_PRINT_STATE(const_copy_constructor);

  default_constructor = copy_constructor;
  map_print_state(default_constructor, "assign operator");

  default_constructor.insert(LIB::make_pair(1, 1));
  default_constructor.insert(LIB::make_pair(1, 2));
  default_constructor.insert(LIB::make_pair(1, 3));
  default_constructor.insert(LIB::make_pair(1, 4));
  default_constructor.insert(LIB::make_pair(1, 5));
  map_print_state(default_constructor, "uniqueness");

  print_data(range_constructor.get_allocator().max_size());
  print_data(const_range_constructor.get_allocator().max_size());

  print_data(copy_constructor.key_comp()(1, 0));
  print_data(copy_constructor.value_comp()(LIB::make_pair(1, 0),
                                           LIB::make_pair(0, 1)));
  print_data(copy_constructor.key_comp()(0, 1));
  print_data(copy_constructor.value_comp()(LIB::make_pair(0, 1),
                                           LIB::make_pair(1, 0)));

  print_data(range_constructor.begin()->first);
  print_data(range_constructor.begin()->second);
  print_data(const_range_constructor.begin()->first);
  print_data(const_range_constructor.begin()->second);

  print_data((--range_constructor.end())->first);
  print_data((--range_constructor.end())->second);
  print_data((--const_range_constructor.end())->first);
  print_data((--const_range_constructor.end())->second);

  print_data(range_constructor.rbegin()->first);
  print_data(range_constructor.rbegin()->second);
  print_data(const_range_constructor.rbegin()->first);
  print_data(const_range_constructor.rbegin()->second);

  print_data((--range_constructor.rend())->first);
  print_data((--range_constructor.rend())->second);
  print_data((--const_range_constructor.rend())->first);
  print_data((--const_range_constructor.rend())->second);

  print_data(range_constructor.size());
  print_data(const_range_constructor.size());

  print_data(range_constructor.max_size());
  print_data(const_range_constructor.max_size());

  print_data(range_constructor.empty());
  print_data(const_range_constructor.empty());

  print_data(range_constructor[default_constructor.size() / 2]);
  print_data(const_range_constructor[const_default_constructor.size() / 2]);

  print_data(range_constructor.at(range_constructor.size() - 1));
  print_data(const_range_constructor.at(const_range_constructor.size() - 1));

  try {
    range_constructor.at(default_constructor.size() + 1);
  } catch (std::out_of_range &e) {
    print_data(e.what());
  }

  try {
    const_range_constructor.at(const_default_constructor.size() + 1);
  } catch (std::out_of_range &e) {
    print_data(e.what());
  }

  print_data(range_constructor.find(data.begin()->first)->first);
  print_data(range_constructor.find(data.begin()->first)->second);
  print_data(const_range_constructor.find(data.begin()->first)->first);
  print_data(const_range_constructor.find(data.begin()->first)->second);

  print_data(range_constructor.find(99) == range_constructor.end());
  print_data(const_range_constructor.find(99) == const_range_constructor.end());

  print_data(range_constructor.count(data.begin()->first));
  print_data(const_range_constructor.count(data.begin()->first));

  print_data(range_constructor.count(99));
  print_data(const_range_constructor.count(99));

  print_data(range_constructor.lower_bound((data.begin() + 2)->first)->first);
  print_data(range_constructor.lower_bound((data.begin() + 2)->first)->second);
  print_data(
      const_range_constructor.lower_bound((data.begin() + 2)->first)->first);
  print_data(
      const_range_constructor.lower_bound((data.begin() + 2)->first)->second);

  print_data(range_constructor.upper_bound((data.begin() + 2)->first)->first);
  print_data(range_constructor.upper_bound((data.begin() + 2)->first)->second);
  print_data(
      const_range_constructor.upper_bound((data.begin() + 2)->first)->first);
  print_data(
      const_range_constructor.upper_bound((data.begin() + 2)->first)->second);

  range_constructor.equal_range((data.begin() + 2)->first);
  const_range_constructor.equal_range((data.begin() + 2)->first);

  range_constructor.insert(LIB::make_pair(50, 50));
  map_print_state(range_constructor, "insert value");

  range_constructor.insert(range_constructor.begin(), LIB::make_pair(51, 51));
  map_print_state(range_constructor, "insert value hint");

  range_constructor.insert(data2.begin(), data2.end());
  map_print_state(range_constructor, "insert range");

  range_constructor.erase(range_constructor.begin());
  map_print_state(range_constructor, "erase position");

  print_data(range_constructor.erase(range_constructor.begin()->first));
  map_print_state(range_constructor, "erase value");

  print_data(range_constructor.erase(55));
  map_print_state(range_constructor, "erase value missing");

  default_constructor.erase(default_constructor.begin(),
                            default_constructor.end());
  map_print_state(default_constructor, "erase range");

  range_constructor.clear();
  map_print_state(range_constructor, "clear");

  range_constructor = copy_constructor;
  print_data(range_constructor == copy_constructor);
  print_data(range_constructor != default_constructor);
  print_data(range_constructor < default_constructor);
  print_data(range_constructor <= copy_constructor);
  print_data(range_constructor > default_constructor);
  print_data(range_constructor >= copy_constructor);

  typename LIB::map<K, V>::iterator it = range_constructor.begin();
  typename LIB::map<K, V>::const_iterator cit = range_constructor.begin();
  print_data(it == cit);
  print_data(it != ++cit);
  // cit = it; // OK
  // it = cit; // NOT OK

  LIB::map<K, V> one(data.begin(), data.end());
  LIB::map<K, V> two(data.rbegin(), data.rend());
  one.swap(two);
  map_print_state(one, "swap 1 one");
  map_print_state(two, "swap 1 two");
  LIB::swap(one, two);
  map_print_state(one, "swap 2 one");
  map_print_state(two, "swap 2 two");
}

template <typename K, typename V>
void map_perf_test(std::string const &key_type, std::string const &val_type) {
  Chrono chrono(key_type + ":" + val_type);
  chrono.begin();

  std::vector<LIB::pair<K, V> > v1;
  for (int i = 0; i < MAP_PERF_BASE_SIZE; ++i) {
    v1.push_back(LIB::make_pair(i, i));
  }

  std::vector<LIB::pair<K, V> > v2;
  for (int i = MAP_PERF_BASE_SIZE; i < MAP_PERF_BASE_SIZE * 2; ++i) {
    v2.push_back(LIB::make_pair(i, i));
  }

  LIB::map<K, V> m(v1.rbegin(), v1.rend());
  chrono.stop("fill constructor");

  m.insert(LIB::make_pair(MAP_PERF_BASE_SIZE + 1, MAP_PERF_BASE_SIZE + 1));
  chrono.stop("insert value");

  m.insert(m.begin(),
           LIB::make_pair(MAP_PERF_BASE_SIZE + 2, MAP_PERF_BASE_SIZE + 2));
  chrono.stop("insert value wrong hint");

  m.insert(--m.end(),
           LIB::make_pair(MAP_PERF_BASE_SIZE + 3, MAP_PERF_BASE_SIZE + 3));
  chrono.stop("insert value correct hint");

  m.insert(v2.rbegin(), v2.rend());
  chrono.stop("insert range");

  m.find(m.begin()->first);
  chrono.stop("find begin");

  m.find(MAP_PERF_BASE_SIZE);
  chrono.stop("find middle");

  m.find((--m.end())->first);
  chrono.stop("find end");

  m.erase(++m.begin());
  chrono.stop("erase position");

  m.erase(MAP_PERF_BASE_SIZE);
  chrono.stop("erase value");

  typename LIB::map<K, V>::iterator end = m.begin();
  for (int i = 0; i < MAP_PERF_BASE_SIZE; ++i) {
    ++end;
  }
  m.erase(m.begin(), end);
  chrono.stop("erase range");

  m.clear();
  chrono.stop("clear");

  chrono.print();
}

void tests_map_impl() {
  print_header("map impl");

  Chrono chrono("map impl");
  chrono.begin();

  MAP_CALL_TEST_FN(map_impl_test, int, int);
  MAP_CALL_TEST_FN(map_impl_test, char, int);
  MAP_CALL_TEST_FN(map_impl_test, testing_struct, int);
  MAP_CALL_TEST_FN(map_impl_test, float, testing_struct);

  chrono.stop("total impl");
  chrono.print();
}

void tests_map_perf() {
  print_header("map perf");

  Chrono chrono("map perf");
  chrono.begin();

  MAP_CALL_TEST_FN(map_perf_test, int, char);
  MAP_CALL_TEST_FN(map_perf_test, testing_struct, int);

  chrono.stop("total perf");
  chrono.print();
}