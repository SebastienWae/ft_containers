#ifndef STACK_HPP
#define STACK_HPP
#include "vector.hpp"

namespace ft {
/* -------------------------------------------------------------------------- */
/*                                    stack                                   */
/* -------------------------------------------------------------------------- */
template <class T, class Container = vector<T> > class stack {
public:
  typedef typename Container::value_type value_type;
  typedef typename Container::size_type size_type;
  typedef typename Container::reference reference;
  typedef typename Container::const_reference const_reference;
  typedef Container container_type;

protected:
  container_type c_;

public:
  /* ------------------------------ constructor ----------------------------- */
  explicit stack(container_type const &c = container_type()) : c_(c) {}

  /* ---------------------------- element access ---------------------------- */
  value_type &top() { return c_.back(); }

  value_type const &top() const { return c_.back(); }

  /* ------------------------------- capacity ------------------------------- */
  bool empty() const { return c_.empty(); }

  size_type size() const { return c_.size(); }

  /* ------------------------------- modifiers ------------------------------ */
  void push(value_type const &x) { c_.push_back(x); }

  void pop() { c_.pop_back(); }

  /* -------------------------------- friend -------------------------------- */
  template <class T_, class C_>
  friend bool operator==(const stack<T_, C_> &lhs, const stack<T_, C_> &rhs);
  template <class T_, class C_>
  friend bool operator<(const stack<T_, C_> &lhs, const stack<T_, C_> &rhs);
};

/* -------------------------------- operator -------------------------------- */
template <class T, class Container>
bool operator==(stack<T, Container> const &lhs,
                stack<T, Container> const &rhs) {
  return lhs.c_ == rhs.c_;
}

template <class T, class Container>
bool operator!=(stack<T, Container> const &lhs,
                stack<T, Container> const &rhs) {
  return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(stack<T, Container> const &lhs, stack<T, Container> const &rhs) {
  return lhs.c_ < rhs.c_;
}

template <class T, class Container>
bool operator<=(stack<T, Container> const &lhs,
                stack<T, Container> const &rhs) {
  return !(rhs < lhs);
}

template <class T, class Container>
bool operator>(stack<T, Container> const &lhs, stack<T, Container> const &rhs) {
  return rhs < lhs;
}

template <class T, class Container>
bool operator>=(stack<T, Container> const &lhs,
                stack<T, Container> const &rhs) {
  return !(lhs < rhs);
}
} // namespace ft

#endif
