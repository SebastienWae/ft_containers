#include <cassert>
#include <cstddef>
#include <stdexcept>

#include <deque>
#include <list>
#include <stack>
#include <vector>

#include "../ft/stack.hpp"
#include "../ft/vector.hpp"

#include "utils/chrono.hpp"
#include "utils/logger.hpp"
#include "utils/testing_struct.hpp"

#ifdef STD
#define STK std::stack
#else
#define STK ft::stack
#endif

template <typename S>
void stack_impl_test(std::string const &container_name, S s) {
  print_data(container_name);

  STK<typename S::value_type, typename S::container_type> o;

  print_data(s.empty());

  for (int i = 0; i < 1000; ++i) {
    s.push(i);
  }

  print_data(s.size());
  print_data(s.empty());
  print_data(s.top());

  for (int i = 0; i < 10; ++i) {
    s.pop();
  }

  print_data(s.size());
  print_data(s.empty());
  print_data(s.top());

  o = s;

  print_data(s == o);
  print_data(s != o);
  print_data(s > o);
  print_data(s >= o);
  print_data(s < o);
  print_data(s <= o);

  o.pop();

  print_data(s == o);
  print_data(s != o);
  print_data(s > o);
  print_data(s >= o);
  print_data(s < o);
  print_data(s <= o);
}

template <typename T> void stack_impl_test(std::string const &type_name) {
  print_data(type_name);

  stack_impl_test("ft vector", STK<T, ft::vector<T> >());
  stack_impl_test("std vector", STK<T, std::vector<T> >());
  stack_impl_test("std list", STK<T, std::list<T> >());
  stack_impl_test("std deque", STK<T, std::deque<T> >());
}

void tests_stack_impl() {
  print_header("stack impl");

  Chrono chrono("stack impl");
  chrono.begin();

  stack_impl_test<int>("int");
  stack_impl_test<float>("float");
  stack_impl_test<testing_struct>("testing_struct");

  chrono.stop("total impl");
  chrono.print();
}