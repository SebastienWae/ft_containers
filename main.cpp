#include "tests/tests.hpp"
#include "tests/utils/logger.hpp"

#include <algorithm>
#include <map>
#include <set>

int main(int argc, char **argv) {
  print_header(argv[0]);

  std::map<std::string, tests_fn> containers;
  NEW_TEST(containers, "vector", &tests_vector_impl, &tests_vector_perf);
  NEW_TEST(containers, "stack", &tests_stack_impl, nullptr);
  NEW_TEST(containers, "set", &tests_set_impl, &tests_set_perf);
  NEW_TEST(containers, "map", &tests_map_impl, &tests_map_perf);

  std::set<std::string> targets;
  bool do_impl_test = true;
  bool do_perf_test = true;

  for (int i = 1; i < argc; ++i) {
    if (argv[i] == std::string("-P")) {
      do_perf_test = false;
    } else if (argv[i] == std::string("-I")) {
      do_impl_test = false;
    } else if (containers.find(argv[i]) != containers.end()) {
      targets.insert(argv[i]);
    }
  }

  for (std::map<std::string, tests_fn>::iterator it = containers.begin();
       it != containers.end(); ++it) {
    if (targets.empty() || targets.find(it->first) != targets.end()) {
      if (do_impl_test && it->second.impl_test != nullptr) {
        it->second.impl_test();
      }
      if (do_perf_test && it->second.perf_test != nullptr) {
        it->second.perf_test();
      }
    }
  }
}