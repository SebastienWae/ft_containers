#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "type_traits.hpp"

#include <iterator>
#include <memory>

namespace ft {
/* -------------------------------------------------------------------------- */
/*                               iterator_traits                              */
/* -------------------------------------------------------------------------- */
template <typename _T_Iterator> struct iterator_traits {
  typedef typename _T_Iterator::iterator_category iterator_category;
  typedef typename _T_Iterator::value_type value_type;
  typedef typename _T_Iterator::difference_type difference_type;
  typedef typename _T_Iterator::pointer pointer;
  typedef typename _T_Iterator::reference reference;
};

template <typename _T> struct iterator_traits<_T *> {
  typedef std::random_access_iterator_tag iterator_category;
  typedef _T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef _T *pointer;
  typedef _T &reference;
};

template <typename _T> struct iterator_traits<_T const *> {
  typedef std::random_access_iterator_tag iterator_category;
  typedef _T value_type;
  typedef std::ptrdiff_t difference_type;
  typedef _T const *pointer;
  typedef _T const &reference;
};

/* -------------------------------------------------------------------------- */
/*                              default iterator                              */
/* -------------------------------------------------------------------------- */
template <typename _T_Iterator, typename _T_Container> class default_iterator {
protected:
  _T_Iterator _m_current;

  typedef std::iterator_traits<_T_Iterator> traits_type;

public:
  typedef _T_Iterator iterator_type;
  typedef typename traits_type::iterator_category iterator_category;
  typedef typename traits_type::value_type value_type;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::reference reference;
  typedef typename traits_type::pointer pointer;

  default_iterator() : _m_current(_T_Iterator()) {}

  explicit default_iterator(iterator_type const &i) : _m_current(i) {}

  template <typename _T_It>
  default_iterator(
      default_iterator<
          _T_It, typename enable_if<
                     (are_same<_T_It, typename _T_Container::pointer>::value),
                     _T_Container>::type> const &i)
      : _m_current(i.base()) {}

  reference operator*() const { return *_m_current; }

  pointer operator->() const { return _m_current; }

  default_iterator &operator++() {
    ++_m_current;
    return *this;
  }

  default_iterator operator++(int) { return default_iterator(_m_current++); }

  default_iterator &operator--() {
    --_m_current;
    return *this;
  }

  default_iterator operator--(int) { return default_iterator(_m_current--); }

  reference operator[](difference_type n) const { return _m_current[n]; }

  default_iterator &operator+=(difference_type n) {
    _m_current += n;
    return *this;
  }

  default_iterator operator+(difference_type n) const {
    return default_iterator(_m_current + n);
  }

  default_iterator &operator-=(difference_type n) {
    _m_current -= n;
    return *this;
  }

  default_iterator operator-(difference_type n) const {
    return default_iterator(_m_current - n);
  }

  const _T_Iterator &base() const { return _m_current; }
};

template <typename _T_IteratorL, typename _T_IteratorR, typename _T_Container>
inline bool
operator==(default_iterator<_T_IteratorL, _T_Container> const &lhs,
           default_iterator<_T_IteratorR, _T_Container> const &rhs) {
  return lhs.base() == rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline bool operator==(default_iterator<_T_Iterator, _T_Container> const &lhs,
                       default_iterator<_T_Iterator, _T_Container> const &rhs) {
  return lhs.base() == rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR, typename _T_Container>
inline bool
operator!=(default_iterator<_T_IteratorL, _T_Container> const &lhs,
           default_iterator<_T_IteratorR, _T_Container> const &rhs) {
  return lhs.base() != rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline bool operator!=(default_iterator<_T_Iterator, _T_Container> const &lhs,
                       default_iterator<_T_Iterator, _T_Container> const &rhs) {
  return lhs.base() != rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR, typename _T_Container>
inline bool operator<(default_iterator<_T_IteratorL, _T_Container> const &lhs,
                      default_iterator<_T_IteratorR, _T_Container> const &rhs) {
  return lhs.base() < rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline bool operator<(default_iterator<_T_Iterator, _T_Container> const &lhs,
                      default_iterator<_T_Iterator, _T_Container> const &rhs) {
  return lhs.base() < rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR, typename _T_Container>
inline bool operator>(default_iterator<_T_IteratorL, _T_Container> const &lhs,
                      default_iterator<_T_IteratorR, _T_Container> const &rhs) {
  return lhs.base() > rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline bool operator>(default_iterator<_T_Iterator, _T_Container> const &lhs,
                      default_iterator<_T_Iterator, _T_Container> const &rhs) {
  return lhs.base() > rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR, typename _T_Container>
inline bool
operator<=(default_iterator<_T_IteratorL, _T_Container> const &lhs,
           default_iterator<_T_IteratorR, _T_Container> const &rhs) {
  return lhs.base() <= rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline bool operator<=(default_iterator<_T_Iterator, _T_Container> const &lhs,
                       default_iterator<_T_Iterator, _T_Container> const &rhs) {
  return lhs.base() <= rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR, typename _T_Container>
inline bool
operator>=(default_iterator<_T_IteratorL, _T_Container> const &lhs,
           default_iterator<_T_IteratorR, _T_Container> const &rhs) {
  return lhs.base() >= rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline bool operator>=(default_iterator<_T_Iterator, _T_Container> const &lhs,
                       default_iterator<_T_Iterator, _T_Container> const &rhs) {
  return lhs.base() >= rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR, typename _T_Container>
inline typename default_iterator<_T_IteratorL, _T_Container>::difference_type
operator-(default_iterator<_T_IteratorL, _T_Container> const &lhs,
          default_iterator<_T_IteratorR, _T_Container> const &rhs) {
  return lhs.base() - rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline typename default_iterator<_T_Iterator, _T_Container>::difference_type
operator-(default_iterator<_T_Iterator, _T_Container> const &lhs,
          default_iterator<_T_Iterator, _T_Container> const &rhs) {
  return lhs.base() - rhs.base();
}

template <typename _T_Iterator, typename _T_Container>
inline default_iterator<_T_Iterator, _T_Container> operator+(
    typename default_iterator<_T_Iterator, _T_Container>::difference_type n,
    default_iterator<_T_Iterator, _T_Container> const &i) {
  return default_iterator<_T_Iterator, _T_Container>(i.base() + n);
}

/* -------------------------------------------------------------------------- */
/*                              reverse_iterator                              */
/* -------------------------------------------------------------------------- */
template <typename _T_Iterator>
class reverse_iterator
    : public std::iterator<
          typename iterator_traits<_T_Iterator>::iterator_category,
          typename iterator_traits<_T_Iterator>::value_type,
          typename iterator_traits<_T_Iterator>::difference_type,
          typename iterator_traits<_T_Iterator>::pointer,
          typename iterator_traits<_T_Iterator>::reference> {
protected:
  _T_Iterator _m_current;

  typedef iterator_traits<_T_Iterator> traits_type;

public:
  typedef _T_Iterator iterator_type;
  typedef typename traits_type::pointer pointer;
  typedef typename traits_type::difference_type difference_type;
  typedef typename traits_type::reference reference;

  reverse_iterator() : _m_current() {}

  explicit reverse_iterator(iterator_type const &x) : _m_current(x) {}

  template <typename _T_It>
  reverse_iterator(reverse_iterator<_T_It> const &x) : _m_current(x.base()) {}

  iterator_type base() const { return _m_current; }

  reference operator*() const {
    _T_Iterator tmp = _m_current;
    return *--tmp;
  }

  pointer operator->() const { return &(operator*()); }

  reverse_iterator &operator++() {
    --_m_current;
    return *this;
  }

  reverse_iterator operator++(int) {
    reverse_iterator tmp = *this;
    --_m_current;
    return tmp;
  }

  reverse_iterator &operator--() {
    ++_m_current;
    return *this;
  }

  reverse_iterator operator--(int) {
    reverse_iterator tmp = *this;
    ++_m_current;
    return tmp;
  }

  reverse_iterator operator+(difference_type n) const {
    return reverse_iterator(_m_current - n);
  }

  reverse_iterator &operator+=(difference_type n) {
    _m_current -= n;
    return *this;
  }

  reverse_iterator operator-(difference_type n) const {
    return reverse_iterator(_m_current + n);
  }

  reverse_iterator &operator-=(difference_type n) {
    _m_current += n;
    return *this;
  }

  reference operator[](difference_type n) const { return *(*this + n); }
};

template <typename _T_Iterator>
inline bool operator==(reverse_iterator<_T_Iterator> const &lhs,
                       reverse_iterator<_T_Iterator> const &rhs) {
  return lhs.base() == rhs.base();
}

template <typename _T_Iterator>
inline bool operator<(reverse_iterator<_T_Iterator> const &lhs,
                      reverse_iterator<_T_Iterator> const &rhs) {
  return rhs.base() < lhs.base();
}

template <typename _T_Iterator>
inline bool operator!=(reverse_iterator<_T_Iterator> const &lhs,
                       reverse_iterator<_T_Iterator> const &rhs) {
  return !(lhs == rhs);
}

template <typename _T_Iterator>
inline bool operator>(reverse_iterator<_T_Iterator> const &lhs,
                      reverse_iterator<_T_Iterator> const &rhs) {
  return rhs < lhs;
}

template <typename _T_Iterator>
inline bool operator<=(reverse_iterator<_T_Iterator> const &lhs,
                       reverse_iterator<_T_Iterator> const &rhs) {
  return !(rhs < lhs);
}

template <typename _T_Iterator>
inline bool operator>=(reverse_iterator<_T_Iterator> const &lhs,
                       reverse_iterator<_T_Iterator> const &rhs) {
  return !(lhs < rhs);
}

template <typename _T_IteratorL, typename _T_IteratorR>
inline bool operator==(reverse_iterator<_T_IteratorL> const &lhs,
                       reverse_iterator<_T_IteratorR> const &rhs) {
  return lhs.base() == rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR>
inline bool operator<(reverse_iterator<_T_IteratorL> const &lhs,
                      reverse_iterator<_T_IteratorR> const &rhs) {
  return lhs.base() > rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR>
inline bool operator!=(reverse_iterator<_T_IteratorL> const &lhs,
                       reverse_iterator<_T_IteratorR> const &rhs) {
  return lhs.base() != rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR>
inline bool operator>(reverse_iterator<_T_IteratorL> const &lhs,
                      reverse_iterator<_T_IteratorR> const &rhs) {
  return lhs.base() < rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR>
inline bool operator<=(reverse_iterator<_T_IteratorL> const &lhs,
                       reverse_iterator<_T_IteratorR> const &rhs) {
  return lhs.base() >= rhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR>
inline bool operator>=(reverse_iterator<_T_IteratorL> const &lhs,
                       reverse_iterator<_T_IteratorR> const &rhs) {
  return lhs.base() <= rhs.base();
}

template <typename _T_Iterator>
inline typename reverse_iterator<_T_Iterator>::difference_type
operator-(reverse_iterator<_T_Iterator> const &lhs,
          reverse_iterator<_T_Iterator> const &rhs) {
  return rhs.base() - lhs.base();
}

template <typename _T_IteratorL, typename _T_IteratorR>
inline typename reverse_iterator<_T_IteratorL>::difference_type
operator-(reverse_iterator<_T_IteratorL> const &lhs,
          reverse_iterator<_T_IteratorR> const &rhs) {
  return rhs.base() - lhs.base();
}

template <typename _T_Iterator>
inline reverse_iterator<_T_Iterator>
operator+(typename reverse_iterator<_T_Iterator>::difference_type n,
          reverse_iterator<_T_Iterator> const &lhs) {
  return reverse_iterator<_T_Iterator>(lhs.base() - n);
}
} // namespace ft

#endif
