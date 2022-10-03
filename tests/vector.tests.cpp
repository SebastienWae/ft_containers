#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <vector>

#include "utils/chrono.hpp"
#include "utils/logger.hpp"
#include "utils/testing_struct.hpp"

#ifdef STD
#define VEC std::vector
#define SWAP std::swap
#else
#include "../ft/vector.hpp"
#define VEC ft::vector
#define SWAP ft::swap
#endif

#define VECTOR_PERF_BASE_SIZE 1000000 // 1 000 000

#define VECTOR_PRINT_STATE(vec) vector_print_state(vec, #vec)
template <typename T>
void vector_print_state(VEC<T> v, std::string const &name) {
  print_data(name);
  print_data(v.size());
  print_data(v.capacity());
  for (typename VEC<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
    print_data(*it);
  }
}

template <typename T> void vector_impl_test(std::string const &type_name) {
  print_data(type_name);

  std::vector<int> data;
  for (int i = 0; i < 10; ++i) {
    data.push_back(i);
  }

  VEC<T> default_constructor;
  VECTOR_PRINT_STATE(default_constructor);

  VEC<T> fill_constructor(2, 2);
  VECTOR_PRINT_STATE(fill_constructor);

  VEC<T> range_constructor(data.begin(), data.end());
  VECTOR_PRINT_STATE(range_constructor);

  VEC<T> copy_constructor(range_constructor);
  VECTOR_PRINT_STATE(copy_constructor);

  VEC<T> const const_default_constructor;
  VECTOR_PRINT_STATE(const_default_constructor);

  VEC<T> const const_fill_constructor(10);
  VECTOR_PRINT_STATE(const_fill_constructor);

  VEC<T> const const_range_constructor(data.begin(), data.end());
  VECTOR_PRINT_STATE(const_range_constructor);

  VEC<T> const const_copy_constructor(const_range_constructor);
  VECTOR_PRINT_STATE(const_copy_constructor);

  fill_constructor = copy_constructor;
  vector_print_state(fill_constructor, "assign operator");

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

  print_data(range_constructor.capacity());
  print_data(const_range_constructor.capacity());

  print_data(range_constructor.empty());
  print_data(const_range_constructor.empty());

  print_data(range_constructor[default_constructor.size() / 2]);
  print_data(const_range_constructor[const_default_constructor.size() / 2]);

  try {
    print_data(range_constructor.at(default_constructor.size() + 1));
  } catch (std::out_of_range &e) {
    print_data(e.what());
  }

  try {
    print_data(
        const_range_constructor.at(const_default_constructor.size() + 1));
  } catch (std::out_of_range &e) {
    print_data(e.what());
  }

  print_data(range_constructor.front());
  print_data(const_range_constructor.front());

  print_data(range_constructor.back());
  print_data(const_range_constructor.back());

  print_data(range_constructor.get_allocator().max_size());
  print_data(const_range_constructor.get_allocator().max_size());

  default_constructor.resize(100);
  vector_print_state(default_constructor, "resize");

  default_constructor.reserve(999);
  vector_print_state(default_constructor, "reserve");

  fill_constructor.assign(5, 0);
  vector_print_state(default_constructor, "assign fill");

  fill_constructor.assign(range_constructor.begin(), range_constructor.end());
  vector_print_state(default_constructor, "assign range");

  range_constructor.push_back(0);
  vector_print_state(range_constructor, "push back");

  range_constructor.pop_back();
  vector_print_state(range_constructor, "pop back");

  range_constructor.insert(range_constructor.begin(), 0);
  vector_print_state(range_constructor, "insert position");

  range_constructor.insert(range_constructor.begin(), 10, 0);
  vector_print_state(range_constructor, "insert fill");

  range_constructor.insert(range_constructor.begin(), copy_constructor.begin(),
                           copy_constructor.end());
  vector_print_state(range_constructor, "insert range");

  range_constructor.erase(range_constructor.begin());
  vector_print_state(range_constructor, "erase");

  range_constructor.clear();
  vector_print_state(range_constructor, "clear");

  range_constructor = copy_constructor;
  print_data(range_constructor == copy_constructor);
  print_data(range_constructor != default_constructor);
  print_data(range_constructor < default_constructor);
  print_data(range_constructor <= copy_constructor);
  print_data(range_constructor > fill_constructor);
  print_data(range_constructor >= copy_constructor);

  typename VEC<T>::iterator it = default_constructor.begin();
  typename VEC<T>::const_iterator cit = default_constructor.begin();
  print_data(it == cit);
  print_data(it != ++cit);
  // cit = it; // OK
  // it = cit; // NOT OK

  VEC<T> one(data.begin(), data.end());
  VEC<T> two(data.rbegin(), data.rend());
  typename VEC<T>::iterator one_begin = one.begin();
  typename VEC<T>::iterator one_end = one.end();
  typename VEC<T>::iterator two_begin = two.begin();
  typename VEC<T>::iterator two_end = two.end();

  one.swap(two);
  assert(one_begin == two.begin());
  assert(one_end == two.end());
  assert(two_begin == one.begin());
  assert(two_end == one.end());
  print_data(*(one_begin + 1));
  print_data(*(two_begin + 1));

  SWAP(one, two);
  assert(one_begin == one.begin());
  assert(one_end == one.end());
  assert(two_begin == two.begin());
  assert(two_end == two.end());
  print_data(*(one_end - 1));
  print_data(*(two_end - 1));
}

template <typename T> void vector_perf_test(std::string const &type_name) {
  Chrono chrono(type_name);
  chrono.begin();

  VEC<T> v(VECTOR_PERF_BASE_SIZE);
  for (int i = 0; i < VECTOR_PERF_BASE_SIZE; ++i) {
    v.push_back(i);
  }
  chrono.stop("loop push back");

  VEC<T> v1(VECTOR_PERF_BASE_SIZE);
  chrono.stop("fill constructor");

  VEC<T> v2(v1.begin(), v1.end());
  chrono.stop("range constructor");

  typename VEC<T>::iterator middle = v1.begin() + (v1.size() / 2);
  v1.insert(middle, v2.begin(), v2.end());
  chrono.stop("range insert");

  v1.resize(VECTOR_PERF_BASE_SIZE * 10);
  chrono.stop("resize increase");

  v1.resize(VECTOR_PERF_BASE_SIZE / 100);
  chrono.stop("resize decrease");

  middle = v1.begin() + (v1.size() / 2);
  v1.erase(middle);
  chrono.stop("erase middle");

  middle = v1.begin() + (v1.size() / 2);
  v1.erase(middle, v1.end());
  chrono.stop("erase 2nd half");

  v1.clear();
  chrono.stop("clear one");

  v2.clear();
  chrono.stop("clear two");

  chrono.print();
}

void tests_vector_impl() {
  print_header("vector impl");

  Chrono chrono("vector impl");
  chrono.begin();

  vector_impl_test<int>("int");
  vector_impl_test<char>("float");
  vector_impl_test<testing_struct>("testing_struct");

  chrono.stop("total impl");
  chrono.print();
}

void tests_vector_perf() {
  print_header("vector perf");

  Chrono chrono("vector perf");
  chrono.begin();

  vector_perf_test<int>("int");
  vector_perf_test<testing_struct>("testing_struct");

  chrono.stop("total perf");
  chrono.print();
}