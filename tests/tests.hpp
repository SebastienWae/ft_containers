#ifndef TESTS_HPP
#define TESTS_HPP

struct tests_fn {
  void (*impl_test)();
  void (*perf_test)();
};

#define NEW_TEST(m, k, i, p)                                                   \
  {                                                                            \
    tests_fn f;                                                                \
    f.impl_test = i;                                                           \
    f.perf_test = p;                                                           \
    m[k] = f;                                                                  \
  }

void tests_vector_impl();
void tests_vector_perf();

void tests_stack_impl();

void tests_set_impl();
void tests_set_perf();

void tests_map_impl();
void tests_map_perf();

#endif
