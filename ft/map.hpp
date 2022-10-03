#ifndef MAP_HPP
#define MAP_HPP

#include "functions.hpp"
#include "tree.hpp"

#include <stdexcept>

namespace ft {
/* -------------------------------------------------------------------------- */
/*                                     map                                    */
/* -------------------------------------------------------------------------- */
template <typename _T_Key, typename _T_Val,
          typename _T_Compare = std::less<_T_Key>,
          typename _T_Allocator = std::allocator<pair<const _T_Key, _T_Val> > >
class map {

  /* ------------------------------- typedefs ------------------------------- */
public:
  typedef _T_Key key_type;
  typedef _T_Val data_type;
  typedef _T_Val mapped_type;
  typedef pair<const _T_Key, _T_Val> value_type;
  typedef _T_Compare key_compare;

  class value_compare : public binary_function<value_type, value_type, bool> {
    friend class map<_T_Key, _T_Val, _T_Compare, _T_Allocator>;

  protected:
    _T_Compare _m_comp;
    value_compare(_T_Compare c) : _m_comp(c) {}

  public:
    bool operator()(value_type const &x, value_type const &y) const {
      return _m_comp(x.first, y.first);
    }
  };

private:
  typedef Rb_tree<key_type, value_type, Select1st<value_type>, key_compare,
                  _T_Allocator>
      _t_tree_type;
  _t_tree_type _m_tree;

public:
  typedef typename _t_tree_type::pointer pointer;
  typedef typename _t_tree_type::const_pointer const_pointer;
  typedef typename _t_tree_type::reference reference;
  typedef typename _t_tree_type::const_reference const_reference;
  typedef typename _t_tree_type::iterator iterator;
  typedef typename _t_tree_type::const_iterator const_iterator;
  typedef typename _t_tree_type::reverse_iterator reverse_iterator;
  typedef typename _t_tree_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _t_tree_type::size_type size_type;
  typedef typename _t_tree_type::difference_type difference_type;
  typedef typename _t_tree_type::allocator_type allocator_type;

  /* ------------------------------ constructor ----------------------------- */
  explicit map(key_compare const &comp = key_compare(),
               allocator_type const &a = allocator_type())
      : _m_tree(comp, a) {}

  template <class _T_InputIterator>
  map(_T_InputIterator first, _T_InputIterator last,
      key_compare const &comp = key_compare(),
      allocator_type const &a = allocator_type())
      : _m_tree(comp, a) {
    _m_tree.insert(first, last);
  }

  map(map<_T_Key, _T_Val, _T_Compare, _T_Allocator> const &x)
      : _m_tree(x._m_tree) {}

  /* ---------------------------- assign operator --------------------------- */
  map<_T_Key, _T_Val, _T_Compare, _T_Allocator> &
  operator=(map<_T_Key, _T_Val, _T_Compare, _T_Allocator> const &x) {
    _m_tree = x._m_tree;
    return *this;
  }

  /* ------------------------------- observer ------------------------------- */

  key_compare key_comp() const { return _m_tree.key_comp(); }
  value_compare value_comp() const { return value_compare(_m_tree.key_comp()); }

  /* ------------------------------- allocator ------------------------------ */
  allocator_type get_allocator() const { return _m_tree.get_allocator(); }

  /* ------------------------------- iterator ------------------------------- */
  iterator begin() { return _m_tree.begin(); }

  const_iterator begin() const { return _m_tree.begin(); }

  iterator end() { return _m_tree.end(); }

  const_iterator end() const { return _m_tree.end(); }

  reverse_iterator rbegin() { return _m_tree.rbegin(); }

  const_reverse_iterator rbegin() const { return _m_tree.rbegin(); }

  reverse_iterator rend() { return _m_tree.rend(); }

  const_reverse_iterator rend() const { return _m_tree.rend(); }

  /* ------------------------------- capacity ------------------------------- */
  bool empty() const { return _m_tree.empty(); }

  size_type size() const { return _m_tree.size(); }

  size_type max_size() const { return _m_tree.max_size(); }

  /* ---------------------------- element access ---------------------------- */
  mapped_type &operator[](key_type const &k) {
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first)) {
      i = insert(i, value_type(k, mapped_type()));
    }
    return (*i).second;
  }

  mapped_type &at(key_type const &k) {
    iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first)) {
      throw std::out_of_range("map::at:  key not found");
    }
    return (*i).second;
  }

  mapped_type const &at(key_type const &k) const {
    const_iterator i = lower_bound(k);
    if (i == end() || key_comp()(k, (*i).first)) {
      throw std::out_of_range("map::at");
    }
    return (*i).second;
  }

  /* ------------------------------- modifier ------------------------------- */
  void swap(map<_T_Key, _T_Val, _T_Compare, _T_Allocator> &x) {
    _m_tree.swap(x._m_tree);
  }

  pair<iterator, bool> insert(value_type const &x) { return _m_tree.insert(x); }

  iterator insert(iterator position, value_type const &x) {
    return _m_tree.insert(position, x);
  }

  template <typename _T_InputIterator>
  void insert(_T_InputIterator first, _T_InputIterator last) {
    _m_tree.insert(first, last);
  }
  void erase(iterator position) { _m_tree.erase(position); }

  size_type erase(key_type const &x) { return _m_tree.erase(x); }

  void erase(iterator first, iterator last) { _m_tree.erase(first, last); }

  void clear() { _m_tree.clear(); }

  /* -------------------------------- lookup -------------------------------- */
  iterator find(key_type const &x) { return _m_tree.find(x); }

  const_iterator find(key_type const &x) const { return _m_tree.find(x); }

  size_type count(key_type const &x) const {
    return _m_tree.find(x) == _m_tree.end() ? 0 : 1;
  }

  iterator lower_bound(key_type const &x) { return _m_tree.lower_bound(x); }

  const_iterator lower_bound(key_type const &x) const {
    return _m_tree.lower_bound(x);
  }

  iterator upper_bound(key_type const &x) { return _m_tree.upper_bound(x); }

  const_iterator upper_bound(key_type const &x) const {
    return _m_tree.upper_bound(x);
  }

  pair<iterator, iterator> equal_range(key_type const &x) {
    return _m_tree.equal_range(x);
  }

  pair<const_iterator, const_iterator> equal_range(key_type const &x) const {
    return _m_tree.equal_range(x);
  }

  /* ------------------------------- operators ------------------------------ */
  template <typename _T_K, typename _T_V, typename _T_C, typename _T_A>
  friend bool operator==(map<_T_K, _T_V, _T_C, _T_A> const &,
                         map<_T_K, _T_V, _T_C, _T_A> const &);
  template <typename _T_K, typename _T_V, typename _T_C, typename _T_A>
  friend bool operator<(map<_T_K, _T_V, _T_C, _T_A> const &,
                        map<_T_K, _T_V, _T_C, _T_A> const &);
};

/* ------------------------------- non members ------------------------------ */
template <typename _T_Key, typename _T_Val, typename _T_Compare,
          typename _T_Alloc>
inline void swap(map<_T_Key, _T_Val, _T_Compare, _T_Alloc> &lhs,
                 map<_T_Key, _T_Val, _T_Compare, _T_Alloc> &rhs) {
  lhs.swap(rhs);
}

template <typename _T_Key, typename _T_Val, typename _T_Compare,
          typename _T_Alloc>
inline bool operator==(map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &lhs,
                       map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &rhs) {
  return lhs._m_tree == rhs._m_tree;
}

template <typename _T_Key, typename _T_Val, typename _T_Compare,
          typename _T_Alloc>
inline bool operator<(map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &lhs,
                      map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &rhs) {
  return lhs._m_tree < rhs._m_tree;
}

template <typename _T_Key, typename _T_Val, typename _T_Compare,
          typename _T_Alloc>
inline bool operator!=(map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &lhs,
                       map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &rhs) {
  return !(lhs == rhs);
}

template <typename _T_Key, typename _T_Val, typename _T_Compare,
          typename _T_Alloc>
inline bool operator>(map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &lhs,
                      map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &rhs) {
  return rhs < lhs;
}

template <typename _T_Key, typename _T_Val, typename _T_Compare,
          typename _T_Alloc>
inline bool operator<=(map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &lhs,
                       map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &rhs) {
  return !(rhs < lhs);
}

template <typename _T_Key, typename _T_Val, typename _T_Compare,
          typename _T_Alloc>
inline bool operator>=(map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &lhs,
                       map<_T_Key, _T_Val, _T_Compare, _T_Alloc> const &rhs) {
  return !(lhs < rhs);
}
} // namespace ft

#endif
