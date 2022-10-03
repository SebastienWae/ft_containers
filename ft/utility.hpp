#ifndef UTILITY_HPP
#define UTILITY_HPP

namespace ft {
/* -------------------------------------------------------------------------- */
/*                                  ft::pair                                  */
/* -------------------------------------------------------------------------- */
template <class T1, class T2> struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  first_type first;
  second_type second;

  pair() : first(first_type()), second(second_type()) {}

  pair(first_type const &a, second_type const &b) : first(a), second(b) {}

  template <class U, class V>
  pair(pair<U, V> const &x) : first(x.first), second(x.second) {}
};

/* -------------------------------- operators ------------------------------- */
template <class T1, class T2>
bool operator==(pair<T1, T2> const &x, pair<T1, T2> const &y) {
  return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
bool operator!=(pair<T1, T2> const &x, pair<T1, T2> const &y) {
  return !(x == y);
}

template <class T1, class T2>
bool operator<(pair<T1, T2> const &x, pair<T1, T2> const &y) {
  return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2>
bool operator>(pair<T1, T2> const &x, pair<T1, T2> const &y) {
  return y < x;
}

template <class T1, class T2>
bool operator>=(pair<T1, T2> const &x, pair<T1, T2> const &y) {
  return !(x < y);
}

template <class T1, class T2>
bool operator<=(pair<T1, T2> const &x, pair<T1, T2> const &y) {
  return !(y < x);
}

/* -------------------------------------------------------------------------- */
/*                                ft::make_pair                               */
/* -------------------------------------------------------------------------- */
template <class T1, class T2> pair<T1, T2> make_pair(T1 x, T2 y) {
  return pair<T1, T2>(x, y);
}

} // namespace ft

#endif
