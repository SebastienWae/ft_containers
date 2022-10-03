#include <cassert>
#include <vector>

#include "utils/chrono.hpp"
#include "utils/logger.hpp"
#include "utils/testing_struct.hpp"

#ifdef STD
#include <set>
#define SET std::set
#define SWAP std::swap
#else
#include "../ft/set.hpp"
#define SET ft::set
#define SWAP ft::swap
#endif

#define SET_PERF_BASE_SIZE 1000000 // 1 000 000

#define SET_PRINT_STATE(set) set_print_state(set, #set)
template <typename T> void set_print_state(SET<T> v, std::string const &name) {
  print_data(name);
  print_data(v.size());
  for (typename SET<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
    print_data(*it);
  }
}

template <typename T> void set_impl_test(std::string const &type_name) {
  print_data(type_name);

  std::vector<int> data;
  for (int i = 0; i < 10; ++i) {
    data.push_back(i);
  }

  std::vector<int> data2;
  for (int i = 10; i < 20; ++i) {
    data2.push_back(i);
  }

  SET<T> default_constructor;
  SET_PRINT_STATE(default_constructor);

  SET<T> range_constructor(data.rbegin(), data.rend());
  SET_PRINT_STATE(range_constructor);

  SET<T> copy_constructor(range_constructor);
  SET_PRINT_STATE(copy_constructor);

  SET<T> const_default_constructor;
  SET_PRINT_STATE(const_default_constructor);

  SET<T> const_range_constructor(data.rbegin(), data.rend());
  SET_PRINT_STATE(const_range_constructor);

  SET<T> const_copy_constructor(const_range_constructor);
  SET_PRINT_STATE(const_copy_constructor);

  default_constructor = copy_constructor;
  set_print_state(default_constructor, "assign operator");

  default_constructor.insert(1);
  default_constructor.insert(1);
  default_constructor.insert(1);
  default_constructor.insert(1);
  default_constructor.insert(1);
  default_constructor.insert(1);
  default_constructor.insert(1);
  set_print_state(default_constructor, "uniqueness");

  print_data(range_constructor.get_allocator().max_size());
  print_data(const_range_constructor.get_allocator().max_size());

  print_data(copy_constructor.key_comp()(1, 0));
  print_data(copy_constructor.value_comp()(1, 0));
  print_data(copy_constructor.key_comp()(0, 1));
  print_data(copy_constructor.value_comp()(0, 1));

  print_data(*range_constructor.begin());
  print_data(*const_range_constructor.begin());

  print_data(*(--range_constructor.end()));
  print_data(*(--const_range_constructor.end()));

  print_data(*range_constructor.rbegin());
  print_data(*const_range_constructor.rbegin());

  print_data(*(--range_constructor.rend()));
  print_data(*(--const_range_constructor.rend()));

  print_data(range_constructor.size());
  print_data(const_range_constructor.size());

  print_data(range_constructor.max_size());
  print_data(const_range_constructor.max_size());

  print_data(range_constructor.empty());
  print_data(const_range_constructor.empty());

  print_data(*range_constructor.find(*data.begin()));
  print_data(*const_range_constructor.find(*data.begin()));

  print_data(range_constructor.find((*--data.end()) + 1) ==
             range_constructor.end());
  print_data(const_range_constructor.find((*--data.end()) + 1) ==
             const_range_constructor.end());

  print_data(range_constructor.count(*data.begin()));
  print_data(const_range_constructor.count(*data.begin()));

  print_data(range_constructor.count((*--data.end()) + 1));
  print_data(const_range_constructor.count((*--data.end()) + 1));

  print_data(*range_constructor.lower_bound(*(data.begin() + 2)));
  print_data(*const_range_constructor.lower_bound(*(data.begin() + 2)));

  print_data(*range_constructor.upper_bound(*(data.begin() + 2)));
  print_data(*const_range_constructor.upper_bound(*(data.begin() + 2)));

  range_constructor.equal_range(*(data.begin() + 2));
  const_range_constructor.equal_range(*(data.begin() + 2));

  range_constructor.insert(50);
  set_print_state(range_constructor, "insert value");

  range_constructor.insert(range_constructor.begin(), 50);
  set_print_state(range_constructor, "insert value hint");

  range_constructor.insert(data2.begin(), data2.end());
  set_print_state(range_constructor, "insert range");

  range_constructor.erase(range_constructor.begin());
  set_print_state(range_constructor, "erase position");

  print_data(range_constructor.erase(*range_constructor.begin()));
  set_print_state(range_constructor, "erase value");

  print_data(range_constructor.erase(55));
  set_print_state(range_constructor, "erase value missing");

  default_constructor.erase(default_constructor.begin(),
                            default_constructor.end());
  set_print_state(default_constructor, "erase range");

  range_constructor.clear();
  set_print_state(range_constructor, "clear");

  range_constructor = copy_constructor;
  print_data(range_constructor == copy_constructor);
  print_data(range_constructor != default_constructor);
  print_data(range_constructor < default_constructor);
  print_data(range_constructor <= copy_constructor);
  print_data(range_constructor > default_constructor);
  print_data(range_constructor >= copy_constructor);

  typename SET<T>::iterator it = range_constructor.begin();
  typename SET<T>::const_iterator cit = range_constructor.begin();
  print_data(it == cit);
  print_data(it != ++cit);
  cit = it;
  it = cit;

  SET<T> one(data.begin(), data.end());
  SET<T> two(data.rbegin(), data.rend());
  one.swap(two);
  set_print_state(one, "swap 1 one");
  set_print_state(two, "swap 1 two");
  SWAP(one, two);
  set_print_state(one, "swap 2 one");
  set_print_state(two, "swap 2 two");
}

template <typename T> void set_perf_test(std::string const &type_name) {
  Chrono chrono(type_name);
  chrono.begin();

  std::vector<T> v1;
  for (int i = 0; i < SET_PERF_BASE_SIZE; ++i) {
    v1.push_back(i);
  }

  std::vector<T> v2;
  for (int i = SET_PERF_BASE_SIZE; i < SET_PERF_BASE_SIZE * 2; ++i) {
    v2.push_back(i);
  }

  SET<T> s(v1.rbegin(), v1.rend());
  chrono.stop("fill constructor");

  s.insert(SET_PERF_BASE_SIZE + 1);
  chrono.stop("insert value");

  s.insert(s.begin(), SET_PERF_BASE_SIZE + 2);
  chrono.stop("insert value wrong hint");

  s.insert(--s.end(), SET_PERF_BASE_SIZE + 3);
  chrono.stop("insert value correct hint");

  s.insert(v2.rbegin(), v2.rend());
  chrono.stop("insert range");

  s.find(*s.begin());
  chrono.stop("find begin");

  s.find(SET_PERF_BASE_SIZE);
  chrono.stop("find middle");

  s.find(*--s.end());
  chrono.stop("find end");

  s.erase(++s.begin());
  chrono.stop("erase position");

  s.erase(SET_PERF_BASE_SIZE);
  chrono.stop("erase value");

  typename SET<T>::iterator end = s.begin();
  for (int i = 0; i < SET_PERF_BASE_SIZE; ++i) {
    ++end;
  }
  s.erase(s.begin(), end);
  chrono.stop("erase range");

  s.clear();
  chrono.stop("clear");

  chrono.print();
}

void tests_set_impl() {
  print_header("set impl");

  Chrono chrono("set impl");
  chrono.begin();

  set_impl_test<int>("int");
  set_impl_test<float>("float");
  set_impl_test<testing_struct>("testing_struct");

  chrono.stop("total impl");
  chrono.print();
}

void tests_set_perf() {
  print_header("set perf");

  Chrono chrono("set perf");
  chrono.begin();

  set_perf_test<int>("int");
  set_perf_test<testing_struct>("testing_struct");

  chrono.stop("total perf");
  chrono.print();
}