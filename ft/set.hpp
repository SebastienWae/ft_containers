#ifndef SET_HPP
#define SET_HPP

#include "functions.hpp"
#include "tree.hpp"

#include <functional>
#include <memory>

namespace ft {
/* -------------------------------------------------------------------------- */
/*                                     set                                    */
/* -------------------------------------------------------------------------- */
template <typename _T_Key, typename _T_Compare = std::less<_T_Key>,
          typename _T_Allocator = std::allocator<_T_Key> >
class set {

  /* ------------------------------- typedefs ------------------------------- */
public:
  typedef _T_Key key_type;
  typedef _T_Key value_type;
  typedef _T_Compare key_compare;
  typedef _T_Compare value_compare;

private:
  typedef Rb_tree<key_type, value_type, Identity<value_type>, key_compare,
                  _T_Allocator>
      _t_tree_type;
  _t_tree_type _m_tree;

public:
  typedef typename _t_tree_type::const_pointer pointer;
  typedef typename _t_tree_type::const_pointer const_pointer;
  typedef typename _t_tree_type::const_reference reference;
  typedef typename _t_tree_type::const_reference const_reference;
  typedef typename _t_tree_type::const_iterator iterator;
  typedef typename _t_tree_type::const_iterator const_iterator;
  typedef typename _t_tree_type::const_reverse_iterator reverse_iterator;
  typedef typename _t_tree_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _t_tree_type::size_type size_type;
  typedef typename _t_tree_type::difference_type difference_type;
  typedef typename _t_tree_type::allocator_type allocator_type;

  /* ------------------------------ constructor ----------------------------- */
  explicit set(_T_Compare const &comp = key_compare(),
               allocator_type const &a = allocator_type())
      : _m_tree(comp, a) {}

  template <typename _T_InputIterator>
  set(_T_InputIterator first, _T_InputIterator last,
      _T_Compare const &comp = key_compare(),
      allocator_type const &a = allocator_type())
      : _m_tree(comp, a) {
    _m_tree.insert(first, last);
  }

  set(set<_T_Key, _T_Compare, _T_Allocator> const &x) : _m_tree(x._m_tree) {}

  /* ---------------------------- assign operator --------------------------- */
  set<_T_Key, _T_Compare, _T_Allocator> &
  operator=(set<_T_Key, _T_Compare, _T_Allocator> const &x) {
    _m_tree = x._m_tree;
    return *this;
  }

  /* ------------------------------- observer ------------------------------- */
  key_compare key_comp() const { return _m_tree.key_comp(); }

  value_compare value_comp() const { return _m_tree.key_comp(); }

  /* ------------------------------- allocator ------------------------------ */
  allocator_type get_allocator() const { return _m_tree.get_allocator(); }

  /* ------------------------------- iterator ------------------------------- */
  iterator begin() const { return _m_tree.begin(); }

  iterator end() const { return _m_tree.end(); }

  reverse_iterator rbegin() const { return _m_tree.rbegin(); }

  reverse_iterator rend() const { return _m_tree.rend(); }

  /* ------------------------------- capacity ------------------------------- */
  bool empty() const { return _m_tree.empty(); }

  size_type size() const { return _m_tree.size(); }

  size_type max_size() const { return _m_tree.max_size(); }

  /* ------------------------------- modifier ------------------------------- */
  void swap(set<_T_Key, _T_Compare, _T_Allocator> &x) {
    _m_tree.swap(x._m_tree);
  }

  pair<iterator, bool> insert(value_type const &x) {
    pair<typename _t_tree_type::iterator, bool> p = _m_tree.insert(x);
    return pair<iterator, bool>(p.first, p.second);
  }

  iterator insert(iterator position, value_type const &x) {
    typedef typename _t_tree_type::iterator _t_tree_iterator;
    return _m_tree.insert((_t_tree_iterator &)position, x);
  }

  template <typename _T_InputIterator>
  void insert(_T_InputIterator first, _T_InputIterator last) {
    _m_tree.insert(first, last);
  }

  void erase(iterator position) {
    typedef typename _t_tree_type::iterator _t_tree_iterator;
    _m_tree.erase((_t_tree_iterator &)position);
  }

  size_type erase(key_type const &x) { return _m_tree.erase(x); }

  void erase(iterator first, iterator last) {
    typedef typename _t_tree_type::iterator _t_tree_iterator;
    _m_tree.erase((_t_tree_iterator &)first, (_t_tree_iterator &)last);
  }

  void clear() { _m_tree.clear(); }

  /* -------------------------------- lookup -------------------------------- */
  iterator find(key_type const &x) const { return _m_tree.find(x); }

  size_type count(key_type const &x) const {
    return _m_tree.find(x) == _m_tree.end() ? 0 : 1;
  }

  iterator lower_bound(key_type const &x) const {
    return _m_tree.lower_bound(x);
  }

  iterator upper_bound(key_type const &x) const {
    return _m_tree.upper_bound(x);
  }

  pair<iterator, iterator> equal_range(key_type const &x) const {
    return _m_tree.equal_range(x);
  }

  /* ------------------------------- operators ------------------------------ */
  template <typename _T_K, typename _T_C, typename _T_A>
  friend bool operator==(set<_T_K, _T_C, _T_A> const &,
                         set<_T_K, _T_C, _T_A> const &);
  template <typename _T_K, typename _T_C, typename _T_A>
  friend bool operator<(set<_T_K, _T_C, _T_A> const &,
                        set<_T_K, _T_C, _T_A> const &);
};

/* ------------------------------- non members ------------------------------ */
template <typename _T_Key, typename _T_Compare, typename _T_Alloc>
inline void swap(set<_T_Key, _T_Compare, _T_Alloc> &lhs,
                 set<_T_Key, _T_Compare, _T_Alloc> &rhs) {
  lhs.swap(rhs);
}

template <typename _T_Key, typename _T_Compare, typename _T_Alloc>
inline bool operator==(set<_T_Key, _T_Compare, _T_Alloc> const &lhs,
                       set<_T_Key, _T_Compare, _T_Alloc> const &rhs) {
  return lhs._m_tree == rhs._m_tree;
}

template <typename _T_Key, typename _T_Compare, typename _T_Alloc>
inline bool operator<(set<_T_Key, _T_Compare, _T_Alloc> const &lhs,
                      set<_T_Key, _T_Compare, _T_Alloc> const &rhs) {
  return lhs._m_tree < rhs._m_tree;
}

template <typename _T_Key, typename _T_Compare, typename _T_Alloc>
inline bool operator!=(set<_T_Key, _T_Compare, _T_Alloc> const &lhs,
                       set<_T_Key, _T_Compare, _T_Alloc> const &rhs) {
  return !(rhs == lhs);
}

template <typename _T_Key, typename _T_Compare, typename _T_Alloc>
inline bool operator>(set<_T_Key, _T_Compare, _T_Alloc> const &lhs,
                      set<_T_Key, _T_Compare, _T_Alloc> const &rhs) {
  return rhs < lhs;
}

template <typename _T_Key, typename _T_Compare, typename _T_Alloc>
inline bool operator<=(set<_T_Key, _T_Compare, _T_Alloc> const &lhs,
                       set<_T_Key, _T_Compare, _T_Alloc> const &rhs) {
  return !(rhs < lhs);
}

template <typename _T_Key, typename _T_Compare, typename _T_Alloc>
inline bool operator>=(set<_T_Key, _T_Compare, _T_Alloc> const &lhs,
                       set<_T_Key, _T_Compare, _T_Alloc> const &rhs) {
  return !(lhs < rhs);
}
} // namespace ft

#endif
