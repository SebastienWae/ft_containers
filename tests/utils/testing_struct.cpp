#include "testing_struct.hpp"
#include <ostream>

testing_struct::testing_struct() : i_(0), c_(0), s_("") {}

testing_struct::testing_struct(int x) : i_(x), c_(x), s_("") {}

testing_struct::testing_struct(int i, char c, std::string const &s)
    : i_(i), c_(c), s_(s) {}

testing_struct::testing_struct(testing_struct const &o)
    : i_(o.i_), c_(o.c_), s_(o.s_) {}

testing_struct &testing_struct::operator=(testing_struct const &o) {
  if (this != &o) {
    i_ = o.i_;
    c_ = o.c_;
    s_ = o.s_;
  }
  return *this;
}

testing_struct::~testing_struct() {}

bool testing_struct::operator==(testing_struct const &o) const {
  return i_ == o.i_ && c_ == o.c_ && s_ == o.s_;
}

bool testing_struct::operator<(testing_struct const &o) const {
  return i_ < o.i_;
}

std::ostream &operator<<(std::ostream &os, testing_struct const &o) {
  return os << "< i_: " << o.i_ << " c_: " << o.c_ << " s_: " << o.s_ << " >";
}