#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace ft {
/* -------------------------------------------------------------------------- */
/*                                 truth type                                 */
/* -------------------------------------------------------------------------- */
struct true_type {};
struct false_type {};

template <bool> struct truth_type {
  typedef false_type type;
};

template <> struct truth_type<true> {
  typedef true_type type;
};

/* -------------------------------------------------------------------------- */
/*                                   traitor                                  */
/* -------------------------------------------------------------------------- */
template <class S, class T> struct traitor {
  enum { value = bool(S::value) || bool(T::value) };
  typedef typename truth_type<value>::type type;
};

/* -------------------------------------------------------------------------- */
/*                                  enable if                                 */
/* -------------------------------------------------------------------------- */
template <bool B, class T = void> struct enable_if {};

template <class T> struct enable_if<true, T> {
  typedef T type;
};

/* -------------------------------------------------------------------------- */
/*                                 is integral                                */
/* -------------------------------------------------------------------------- */
template <class T> struct is_integral {
  enum { value = 0 };
};

template <> struct is_integral<bool> {
  enum { value = 1 };
};

template <> struct is_integral<char> {
  enum { value = 1 };
};

template <> struct is_integral<wchar_t> {
  enum { value = 1 };
};

template <> struct is_integral<short> {
  enum { value = 1 };
};

template <> struct is_integral<int> {
  enum { value = 1 };
};

template <> struct is_integral<long> {
  enum { value = 1 };
};

template <> struct is_integral<long long> {
  enum { value = 1 };
};

template <> struct is_integral<unsigned long long> {
  enum { value = 1 };
};

/* -------------------------------------------------------------------------- */
/*                                 is floating                                */
/* -------------------------------------------------------------------------- */
template <typename T> struct is_floating {
  enum { value = 0 };
  typedef false_type type;
};

template <> struct is_floating<float> {
  enum { value = 1 };
  typedef true_type type;
};

template <> struct is_floating<double> {
  enum { value = 1 };
  typedef true_type type;
};

template <> struct is_floating<long double> {
  enum { value = 1 };
  typedef true_type type;
};

/* -------------------------------------------------------------------------- */
/*                                is arithmetic                               */
/* -------------------------------------------------------------------------- */
template <typename T>
struct is_arithmetic : public traitor<is_integral<T>, is_floating<T> > {};

/* -------------------------------------------------------------------------- */
/*                                are same type                               */
/* -------------------------------------------------------------------------- */
template <typename, typename> struct are_same {
  enum { value = 0 };
  typedef false_type type;
};

template <typename T> struct are_same<T, T> {
  enum { value = 1 };
  typedef true_type type;
};
} // namespace ft

#endif
