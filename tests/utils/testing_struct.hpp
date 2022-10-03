#ifndef TESTING_STRUCT_HPP
#define TESTING_STRUCT_HPP

#include <string>

struct testing_struct {
  int i_;
  char c_;
  std::string s_;

  testing_struct();

  testing_struct(int x);

  testing_struct(int i, char c, std::string const &s);

  testing_struct(testing_struct const &o);

  testing_struct &operator=(testing_struct const &o);

  ~testing_struct();

  bool operator==(testing_struct const &o) const;
  bool operator<(testing_struct const &o) const;
};
std::ostream &operator<<(std::ostream &os, testing_struct const &o);

#endif
