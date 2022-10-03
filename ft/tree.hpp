#ifndef TREE_HPP
#define TREE_HPP

#include "algorithm.hpp"
#include "iterator.hpp"
#include "utility.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>

namespace ft {
enum _E_Rb_tree_node_color { RBT_RED, RBT_BLACK };

/* -------------------------------------------------------------------------- */
/*                               tree node base                               */
/* -------------------------------------------------------------------------- */
struct Rb_tree_node_base {
  typedef Rb_tree_node_base *_t_base_ptr;
  typedef Rb_tree_node_base const *_t_const_base_ptr;

  _E_Rb_tree_node_color _m_color;
  _t_base_ptr _m_parent;
  _t_base_ptr _m_left;
  _t_base_ptr _m_right;

  static _t_base_ptr min(_t_base_ptr x);

  static _t_const_base_ptr min(_t_const_base_ptr x);

  static _t_base_ptr max(_t_base_ptr x);

  static _t_const_base_ptr max(_t_const_base_ptr x);
};

/* -------------------------------------------------------------------------- */
/*                                  tree node                                 */
/* -------------------------------------------------------------------------- */
template <typename _T_Val> struct Rb_tree_node : public Rb_tree_node_base {
  typedef Rb_tree_node<_T_Val> *_t_node_ptr;
  _T_Val _m_value;
};

/* -------------------------------------------------------------------------- */
/*                                  utilities                                 */
/* -------------------------------------------------------------------------- */
Rb_tree_node_base *Rb_tree_node_increment(Rb_tree_node_base *x);

Rb_tree_node_base const *Rb_tree_node_increment(Rb_tree_node_base const *x);

Rb_tree_node_base *Rb_tree_node_decrement(Rb_tree_node_base *x);

Rb_tree_node_base const *Rb_tree_node_decrement(Rb_tree_node_base const *x);

void Rb_tree_insert_and_rebalance(bool const insert_left, Rb_tree_node_base *x,
                                  Rb_tree_node_base *p,
                                  Rb_tree_node_base &header);

Rb_tree_node_base *Rb_tree_rebalance_for_erase(Rb_tree_node_base *const z,
                                               Rb_tree_node_base &header);

/* -------------------------------------------------------------------------- */
/*                                  iterator                                  */
/* -------------------------------------------------------------------------- */
template <typename _T_Val> struct Rb_tree_iterator {
  typedef _T_Val value_type;
  typedef _T_Val &reference;
  typedef _T_Val *pointer;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;

  typedef Rb_tree_iterator<_T_Val> _t_self;
  typedef Rb_tree_node_base::_t_base_ptr _t_base_ptr;
  typedef Rb_tree_node<_T_Val> *_t_node_ptr;

  _t_base_ptr _m_node;

  Rb_tree_iterator() : _m_node() {}

  Rb_tree_iterator(_t_node_ptr x) : _m_node(x) {}

  reference operator*() const {
    return static_cast<_t_node_ptr>(_m_node)->_m_value;
  }

  pointer operator->() const {
    return &static_cast<_t_node_ptr>(_m_node)->_m_value;
  }

  _t_self &operator++() {
    _m_node = Rb_tree_node_increment(_m_node);
    return *this;
  }

  _t_self operator++(int) {
    _t_self tmp = *this;
    _m_node = Rb_tree_node_increment(_m_node);
    return tmp;
  }

  _t_self &operator--() {
    _m_node = Rb_tree_node_decrement(_m_node);
    return *this;
  }

  _t_self operator--(int) {
    _t_self tmp = *this;
    _m_node = Rb_tree_node_decrement(_m_node);
    return tmp;
  }

  friend bool operator==(_t_self const &lhs, _t_self const &rhs) {
    return lhs._m_node == rhs._m_node;
  }

  friend bool operator!=(_t_self const &lhs, _t_self const &rhs) {
    return lhs._m_node != rhs._m_node;
  }
};

/* -------------------------------------------------------------------------- */
/*                               const iterator                               */
/* -------------------------------------------------------------------------- */
template <typename _T_Val> struct Rb_tree_const_iterator {
  typedef _T_Val value_type;
  typedef _T_Val const &reference;
  typedef _T_Val const *pointer;

  typedef Rb_tree_iterator<_T_Val> iterator;

  typedef std::bidirectional_iterator_tag iterator_category;
  typedef std::ptrdiff_t difference_type;

  typedef Rb_tree_const_iterator<_T_Val> _t_self;
  typedef Rb_tree_node_base::_t_const_base_ptr _t_base_ptr;
  typedef Rb_tree_node<_T_Val> const *_t_node_ptr;

  _t_base_ptr _m_node;

  Rb_tree_const_iterator() : _m_node() {}

  Rb_tree_const_iterator(_t_node_ptr x) : _m_node(x) {}

  Rb_tree_const_iterator(iterator const &it) : _m_node(it._m_node) {}

  reference operator*() const {
    return static_cast<_t_node_ptr>(_m_node)->_m_value;
  }

  pointer operator->() const {
    return &static_cast<_t_node_ptr>(_m_node)->_m_value;
  }

  _t_self &operator++() {
    _m_node = Rb_tree_node_increment(_m_node);
    return *this;
  }

  _t_self operator++(int) {
    _t_self tmp = *this;
    _m_node = Rb_tree_node_increment(_m_node);
    return tmp;
  }

  _t_self &operator--() {
    _m_node = Rb_tree_node_decrement(_m_node);
    return *this;
  }

  _t_self operator--(int) {
    _t_self tmp = *this;
    _m_node = Rb_tree_node_decrement(_m_node);
    return tmp;
  }

  friend bool operator==(_t_self const &lhs, _t_self const &rhs) {
    return lhs._m_node == rhs._m_node;
  }

  friend bool operator!=(_t_self const &lhs, _t_self const &rhs) {
    return lhs._m_node != rhs._m_node;
  }
};

/* -------------------------------------------------------------------------- */
/*                                    tree                                    */
/* -------------------------------------------------------------------------- */
template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc = std::allocator<_T_Val> >
class Rb_tree {

  /* -------------------------------- typedef ------------------------------- */
  typedef typename _T_Alloc::template rebind<Rb_tree_node<_T_Val> >::other
      _t_node_allocator;

  typedef Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> _t_self;
  typedef Rb_tree_node_base *_t_base_ptr;
  typedef Rb_tree_node_base const *_t_const_base_ptr;
  typedef Rb_tree_node<_T_Val> Rb_tree_node;

public:
  typedef _T_Key key_type;
  typedef _T_Val value_type;
  typedef _T_Alloc allocator_type;
  typedef value_type *pointer;
  typedef value_type const *const_pointer;
  typedef value_type &reference;
  typedef value_type const &const_reference;
  typedef Rb_tree_node *_t_node_ptr;
  typedef Rb_tree_node const *_t_const_node_ptr;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef Rb_tree_iterator<value_type> iterator;
  typedef Rb_tree_const_iterator<value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /* ------------------------------- allocator ------------------------------ */
  allocator_type get_allocator() const {
    return *static_cast<_t_node_allocator const *>(&this->_m_impl);
  }

protected:
  Rb_tree_node *_allocate_node() {
    return this->_m_impl._t_node_allocator::allocate(1);
  }

  void _deallocate_node(Rb_tree_node *p) {
    this->_m_impl._t_node_allocator::deallocate(p, 1);
  }

  _t_node_ptr _construct_node(const value_type &x) {
    _t_node_ptr tmp = _allocate_node();
    try {
      get_allocator().construct(&tmp->_m_value, x);
    } catch (...) {
      _deallocate_node(tmp);
      throw;
    }
    return tmp;
  }

  _t_node_ptr _clone_node(_t_const_node_ptr x) {
    _t_node_ptr tmp = _construct_node(x->_m_value);
    tmp->_m_color = x->_m_color;
    tmp->_m_left = NULL;
    tmp->_m_right = NULL;
    return tmp;
  }

  void _destroy_node(_t_node_ptr p) {
    get_allocator().destroy(&p->_m_value);
    _deallocate_node(p);
  }

  /* ---------------------------- implementation ---------------------------- */
  template <typename _T_Key_compare>
  struct Rb_tree_impl : public _t_node_allocator {
    _T_Key_compare _m_key_compare;
    Rb_tree_node_base _m_header;
    size_type _m_node_count;

    Rb_tree_impl(_t_node_allocator const &a = _t_node_allocator(),
                 _T_Key_compare const &comp = _T_Key_compare())
        : _t_node_allocator(a), _m_key_compare(comp), _m_node_count(0) {
      this->_m_header._m_color = RBT_RED;
      this->_m_header._m_parent = NULL;
      this->_m_header._m_left = &this->_m_header;
      this->_m_header._m_right = &this->_m_header;
    }
  };

  Rb_tree_impl<_T_Compare> _m_impl;

  /* -------------------------------- helpers ------------------------------- */
  _t_base_ptr &_root() { return this->_m_impl._m_header._m_parent; }

  _t_const_base_ptr _root() const { return this->_m_impl._m_header._m_parent; }

  _t_base_ptr &_leftmost() { return this->_m_impl._m_header._m_left; }

  _t_const_base_ptr _leftmost() const {
    return this->_m_impl._m_header._m_left;
  }

  _t_base_ptr &_rightmost() { return this->_m_impl._m_header._m_right; }

  _t_const_base_ptr _rightmost() const {
    return this->_m_impl._m_header._m_right;
  }

  _t_node_ptr _begin() {
    return static_cast<_t_node_ptr>(this->_m_impl._m_header._m_parent);
  }

  _t_const_node_ptr _begin() const {
    return static_cast<_t_const_node_ptr>(this->_m_impl._m_header._m_parent);
  }

  _t_node_ptr _end() {
    return static_cast<_t_node_ptr>(&this->_m_impl._m_header);
  }

  _t_const_node_ptr _end() const {
    return static_cast<_t_const_node_ptr>(&this->_m_impl._m_header);
  }

  static const_reference _node_value(_t_const_node_ptr x) {
    return x->_m_value;
  }

  static _T_Key const &_node_key(_t_const_node_ptr x) {
    return _T_KeyOfValue()(_node_value(x));
  }

  static _t_node_ptr _left(_t_base_ptr x) {
    return static_cast<_t_node_ptr>(x->_m_left);
  }

  static _t_const_node_ptr _left(_t_const_base_ptr x) {
    return static_cast<_t_const_node_ptr>(x->_m_left);
  }

  static _t_node_ptr _right(_t_base_ptr x) {
    return static_cast<_t_node_ptr>(x->_m_right);
  }

  static _t_const_node_ptr _right(_t_const_base_ptr x) {
    return static_cast<_t_const_node_ptr>(x->_m_right);
  }

  static const_reference _node_value(_t_const_base_ptr x) {
    return static_cast<_t_const_node_ptr>(x)->_m_value;
  }

  static _T_Key const &_node_key(_t_const_base_ptr x) {
    return _T_KeyOfValue()(_node_value(x));
  }

  static _t_base_ptr _min(_t_base_ptr x) { return Rb_tree_node_base::min(x); }

  static _t_const_base_ptr _min(_t_const_base_ptr x) {
    return Rb_tree_node_base::min(x);
  }

  static _t_base_ptr _max(_t_base_ptr x) { return Rb_tree_node_base::max(x); }

  static _t_const_base_ptr _max(_t_const_base_ptr x) {
    return Rb_tree_node_base::max(x);
  }

  /* ------------------------------ constructor ----------------------------- */
public:
  Rb_tree() {}

  Rb_tree(_T_Compare const &comp) : _m_impl(allocator_type(), comp) {}

  Rb_tree(_T_Compare const &comp, allocator_type const &a) : _m_impl(a, comp) {}

  Rb_tree(_t_self const &x)
      : _m_impl(x.get_allocator(), x._m_impl._m_key_compare) {
    if (x._root() != NULL) {
      _root() = _copy(x._begin(), _end());
      _leftmost() = _min(_root());
      _rightmost() = _max(_root());
      this->_m_impl._m_node_count = x._m_impl._m_node_count;
    }
  }

  /* ------------------------------ destructor ------------------------------ */
  ~Rb_tree() { _erase(_begin()); }

  /* ---------------------------- assign operator --------------------------- */
  _t_self &operator=(_t_self const &x) {
    if (this != &x) {
      clear();
      this->_m_impl._m_key_compare = x._m_impl._m_key_compare;
      if (x._root() != NULL) {
        _root() = _copy(x._begin(), _end());
        _leftmost() = _min(_root());
        _rightmost() = _max(_root());
        this->_m_impl._m_node_count = x._m_impl._m_node_count;
      }
    }
    return *this;
  }

  /* ------------------------------- observer ------------------------------- */
  _T_Compare key_comp() const { return this->_m_impl._m_key_compare; }

  /* ------------------------------- iterator ------------------------------- */
  iterator begin() {
    return static_cast<_t_node_ptr>(this->_m_impl._m_header._m_left);
  }

  const_iterator begin() const {
    return static_cast<_t_const_node_ptr>(this->_m_impl._m_header._m_left);
  }

  iterator end() { return static_cast<_t_node_ptr>(&this->_m_impl._m_header); }

  const_iterator end() const {
    return static_cast<_t_const_node_ptr>(&this->_m_impl._m_header);
  }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  /* ------------------------------- capacity ------------------------------- */
  bool empty() const { return this->_m_impl._m_node_count == 0; }

  size_type size() const { return this->_m_impl._m_node_count; }

  size_type max_size() const {
    return std::min<size_type>(this->_m_impl._t_node_allocator::max_size(),
                               std::numeric_limits<difference_type>::max());
  }

  /* ------------------------------- modifier ------------------------------- */
private:
  iterator _insert(_t_base_ptr x, _t_base_ptr y, value_type const &v) {
    bool insert_left =
        (x != NULL || y == _end() ||
         this->_m_impl._m_key_compare(_T_KeyOfValue()(v), _node_key(y)));
    _t_node_ptr z = _construct_node(v);
    Rb_tree_insert_and_rebalance(insert_left, z, y, this->_m_impl._m_header);
    ++this->_m_impl._m_node_count;
    return iterator(z);
  }

  _t_node_ptr _copy(_t_const_node_ptr x, _t_node_ptr p) {
    _t_node_ptr top = _clone_node(x);
    top->_m_parent = p;
    try {
      if (x->_m_right) {
        top->_m_right = _copy(_right(x), top);
      }
      p = top;
      x = _left(x);
      while (x != NULL) {
        _t_node_ptr y = _clone_node(x);
        p->_m_left = y;
        y->_m_parent = p;
        if (x->_m_right) {
          y->_m_right = _copy(_right(x), y);
        }
        p = y;
        x = _left(x);
      }
    } catch (...) {
      _erase(top);
      throw;
    }
    return top;
  }

  void _erase(_t_node_ptr x) {
    while (x != NULL) {
      _erase(_right(x));
      _t_node_ptr y = _left(x);
      _destroy_node(x);
      x = y;
    }
  }

public:
  pair<iterator, bool> insert(value_type const &v) {
    _t_node_ptr x = _begin();
    _t_node_ptr y = _end();
    bool comp = true;
    while (x != NULL) {
      y = x;
      comp = this->_m_impl._m_key_compare(_T_KeyOfValue()(v), _node_key(x));
      x = comp ? _left(x) : _right(x);
    }
    iterator j = iterator(y);
    if (comp) {
      if (j == begin()) {
        return pair<iterator, bool>(_insert(x, y, v), true);
      } else {
        --j;
      }
    }
    if (this->_m_impl._m_key_compare(_node_key(j._m_node),
                                     _T_KeyOfValue()(v))) {
      return pair<iterator, bool>(_insert(x, y, v), true);
    }
    return pair<iterator, bool>(j, false);
  }

  iterator insert(iterator position, value_type const &v) {
    if (position._m_node == _end()) {
      if (size() > 0 && this->_m_impl._m_key_compare(_node_key(_rightmost()),
                                                     _T_KeyOfValue()(v))) {
        return _insert(0, _rightmost(), v);
      }
      return insert(v).first;
    }
    if (this->_m_impl._m_key_compare(_T_KeyOfValue()(v),
                                     _node_key(position._m_node))) {
      iterator before = position;
      if (position._m_node == _leftmost()) {
        return _insert(_leftmost(), _leftmost(), v);
      }
      if (this->_m_impl._m_key_compare(_node_key((--before)._m_node),
                                       _T_KeyOfValue()(v))) {
        if (_right(before._m_node) == NULL) {
          return _insert(0, before._m_node, v);
        }
        return _insert(position._m_node, position._m_node, v);
      }
      return insert(v).first;
    }
    if (this->_m_impl._m_key_compare(_node_key(position._m_node),
                                     _T_KeyOfValue()(v))) {
      iterator after = position;
      if (position._m_node == _rightmost()) {
        return _insert(0, _rightmost(), v);
      }
      if (this->_m_impl._m_key_compare(_T_KeyOfValue()(v),
                                       _node_key((++after)._m_node))) {
        if (_right(position._m_node) == NULL) {
          return _insert(0, position._m_node, v);
        }
        return _insert(after._m_node, after._m_node, v);
      }
      return insert(v).first;
    }
    return position;
  }

  template <typename _T_InputIterator>
  void insert(_T_InputIterator first, _T_InputIterator last) {
    for (; first != last; ++first) {
      insert(end(), *first);
    }
  }

  void erase(iterator position) {
    _t_node_ptr y = static_cast<_t_node_ptr>(
        Rb_tree_rebalance_for_erase(position._m_node, this->_m_impl._m_header));
    _destroy_node(y);
    --this->_m_impl._m_node_count;
  }

  size_type erase(key_type const &x) {
    pair<iterator, iterator> p = equal_range(x);
    size_type n = std::distance(p.first, p.second);
    erase(p.first, p.second);
    return n;
  }

  void erase(iterator first, iterator last) {
    if (first == begin() && last == end()) {
      clear();
    } else {
      while (first != last) {
        erase(first++);
      }
    }
  }

  void clear() {
    _erase(_begin());
    _leftmost() = _end();
    _root() = NULL;
    _rightmost() = _end();
    this->_m_impl._m_node_count = 0;
  }

  void swap(_t_self &t) {
    if (_root() == NULL) {
      if (t._root() != NULL) {
        _root() = t._root();
        _leftmost() = t._leftmost();
        _rightmost() = t._rightmost();
        _root()->_m_parent = _end();
        t._root() = NULL;
        t._leftmost() = t._end();
        t._rightmost() = t._end();
      }
    } else if (t._root() == NULL) {
      t._root() = _root();
      t._leftmost() = _leftmost();
      t._rightmost() = _rightmost();
      t._root()->_m_parent = t._end();
      _root() = NULL;
      _leftmost() = _end();
      _rightmost() = _end();
    } else {
      std::swap(_root(), t._root());
      std::swap(_leftmost(), t._leftmost());
      std::swap(_rightmost(), t._rightmost());
      _root()->_m_parent = _end();
      t._root()->_m_parent = t._end();
    }
    std::swap(this->_m_impl._m_node_count, t._m_impl._m_node_count);
    std::swap(this->_m_impl._m_key_compare, t._m_impl._m_key_compare);
  }

  /* -------------------------------- lookup -------------------------------- */
  iterator find(_T_Key const &k) {
    _t_node_ptr x = _begin();
    _t_node_ptr y = _end();
    while (x != NULL) {
      if (!this->_m_impl._m_key_compare(_node_key(x), k)) {
        y = x, x = _left(x);
      } else {
        x = _right(x);
      }
    }
    iterator j = iterator(y);
    return (j == end() || this->_m_impl._m_key_compare(k, _node_key(j._m_node)))
               ? end()
               : j;
  }

  const_iterator find(_T_Key const &k) const {
    _t_const_node_ptr x = _begin();
    _t_const_node_ptr y = _end();
    while (x != NULL) {
      if (!this->_m_impl._m_key_compare(_node_key(x), k)) {
        y = x, x = _left(x);
      } else {
        x = _right(x);
      }
    }
    const_iterator j = const_iterator(y);
    return (j == end() || this->_m_impl._m_key_compare(k, _node_key(j._m_node)))
               ? end()
               : j;
  }

  size_type count(_T_Key const &k) const {
    pair<const_iterator, const_iterator> p = equal_range(k);
    const size_type n = std::distance(p.first, p.second);
    return n;
  }

  iterator lower_bound(_T_Key const &k) {
    _t_node_ptr x = _begin();
    _t_node_ptr y = _end();
    while (x != NULL) {
      if (!this->_m_impl._m_key_compare(_node_key(x), k)) {
        y = x, x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return iterator(y);
  }

  const_iterator lower_bound(_T_Key const &k) const {
    _t_const_node_ptr x = _begin();
    _t_const_node_ptr y = _end();
    while (x != NULL) {
      if (!this->_m_impl._m_key_compare(_node_key(x), k)) {
        y = x, x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return const_iterator(y);
  }

  iterator upper_bound(_T_Key const &k) {
    _t_node_ptr x = _begin();
    _t_node_ptr y = _end();
    while (x != NULL) {
      if (this->_m_impl._m_key_compare(k, _node_key(x))) {
        y = x, x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return iterator(y);
  }

  const_iterator upper_bound(_T_Key const &k) const {
    _t_const_node_ptr x = _begin();
    _t_const_node_ptr y = _end();
    while (x != NULL) {
      if (this->_m_impl._m_key_compare(k, _node_key(x))) {
        y = x, x = _left(x);
      } else {
        x = _right(x);
      }
    }
    return const_iterator(y);
  }

  pair<iterator, iterator> equal_range(_T_Key const &k) {
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
  }

  pair<const_iterator, const_iterator> equal_range(_T_Key const &k) const {
    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
  }
};

/* -------------------------- non-member functions -------------------------- */
template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc>
inline void
swap(Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> &lhs,
     Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> &rhs) {
  lhs.swap(rhs);
}

template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc>
inline bool operator==(
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &lhs,
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &rhs) {
  return lhs.size() == rhs.size() &&
         ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc>
inline bool operator<(
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &lhs,
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &rhs) {
  return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                     rhs.end());
}

template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc>
inline bool operator!=(
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &lhs,
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &rhs) {
  return !(lhs == rhs);
}

template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc>
inline bool operator>(
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &lhs,
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &rhs) {
  return rhs < lhs;
}

template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc>
inline bool operator<=(
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &lhs,
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &rhs) {
  return !(rhs < lhs);
}

template <typename _T_Key, typename _T_Val, typename _T_KeyOfValue,
          typename _T_Compare, typename _T_Alloc>
inline bool operator>=(
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &lhs,
    Rb_tree<_T_Key, _T_Val, _T_KeyOfValue, _T_Compare, _T_Alloc> const &rhs) {
  return !(lhs < rhs);
}
} // namespace ft

#endif
