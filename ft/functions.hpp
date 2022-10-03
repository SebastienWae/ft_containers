#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

template <class _T_Arg, class _T_Result> struct unary_function {
  typedef _T_Arg argument_type;
  typedef _T_Result result_type;
};

template <class _T_Arg1, class _T_Arg2, class _T_Result>
struct binary_function {
  typedef _T_Arg1 first_argument_type;
  typedef _T_Arg2 second_argument_type;
  typedef _T_Result result_type;
};

template <class _T> struct Identity : public unary_function<_T, _T> {
  _T const &operator()(_T const &x) const { return x; }
};

template <class _T_Pair>
struct Select1st
    : public unary_function<_T_Pair, typename _T_Pair::first_type> {
  typename _T_Pair::first_type const &operator()(_T_Pair const &__x) const {
    return __x.first;
  }
};

#endif
