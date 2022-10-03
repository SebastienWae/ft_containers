#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "allocator.hpp"
#include "iterator.hpp"
#include "type_traits.hpp"

#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>

namespace ft {
/* -------------------------------------------------------------------------- */
/*                                 vector base                                */
/* -------------------------------------------------------------------------- */
template <typename _T_Value, typename _T_Allocator> struct vector_base {
  struct vector_impl : public _T_Allocator {
    _T_Value *_m_start;
    _T_Value *_m_finish;
    _T_Value *_m_end_of_storage;

    vector_impl(_T_Allocator const &allocator)
        : _T_Allocator(allocator), _m_start(NULL), _m_finish(NULL),
          _m_end_of_storage(NULL) {}
  };

  typedef _T_Allocator allocator_type;

  vector_impl _m_impl;

  vector_base(allocator_type const &allocator) : _m_impl(allocator) {}

  vector_base(std::size_t size, allocator_type const &allocator)
      : _m_impl(allocator) {
    _m_impl._m_start = allocate(size);
    _m_impl._m_finish = _m_impl._m_start;
    _m_impl._m_end_of_storage = _m_impl._m_start + size;
  }

  ~vector_base() {
    deallocate(_m_impl._m_start, _m_impl._m_end_of_storage - _m_impl._m_start);
  }

  _T_Value *allocate(std::size_t size) { return _m_impl.allocate(size); }

  void deallocate(_T_Value *pointer, std::size_t size) {
    if (pointer) {
      _m_impl.deallocate(pointer, size);
    }
  }
};

/* -------------------------------------------------------------------------- */
/*                                   vector                                   */
/* -------------------------------------------------------------------------- */
template <typename _T_Value, typename _T_Allocator = std::allocator<_T_Value> >
class vector : protected vector_base<_T_Value, _T_Allocator> {
  /* -------------------------------- typedef ------------------------------- */
  typedef vector_base<_T_Value, _T_Allocator> _t_vector_base;
  typedef vector<_T_Value, _T_Allocator> _t_self;

public:
  typedef _T_Value value_type;
  typedef typename _t_vector_base::allocator_type allocator_type;
  typedef typename allocator_type::pointer pointer;
  typedef typename allocator_type::const_pointer const_pointer;
  typedef typename allocator_type::reference reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type size_type;
  typedef typename allocator_type::difference_type difference_type;
  typedef default_iterator<pointer, _t_self> iterator;
  typedef default_iterator<const_pointer, _t_self> const_iterator;
  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;

  /* ------------------------------ constructor ----------------------------- */
  explicit vector(allocator_type const &alloc = allocator_type())
      : _t_vector_base(alloc) {}

  vector(size_type n, value_type const &val = value_type(),
         allocator_type const &alloc = allocator_type())
      : _t_vector_base(n, alloc) {
    uninitialized_fill_n_a(this->_m_impl._m_start, n, val,
                           this->get_allocator());
    this->_m_impl._m_finish = this->_m_impl._m_start + n;
  }

  vector(_t_self const &x) : _t_vector_base(x.size(), x.get_allocator()) {
    this->_m_impl._m_finish = uninitialized_copy_a(
        x.begin(), x.end(), this->_m_impl._m_start, this->get_allocator());
  }

protected:
  template <typename _T_InputIterator>
  void _range_initialize(_T_InputIterator first, _T_InputIterator last,
                         std::input_iterator_tag) {
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  template <typename _T_ForwardIterator>
  void _range_initialize(_T_ForwardIterator first, _T_ForwardIterator last,
                         std::forward_iterator_tag) {
    size_type const n = std::distance(first, last);
    this->_m_impl._m_start = this->allocate(n);
    this->_m_impl._m_end_of_storage = this->_m_impl._m_start + n;
    this->_m_impl._m_finish = uninitialized_copy_a(
        first, last, this->_m_impl._m_start, this->get_allocator());
  }

public:
  /*
    ft::enable_if is used to enable SFINAE when the vector constructor have
    two arguments of similar arithmetic type.

    We cannot determine the capacity beforehand if InputIterator is not at least
    a 'forward iterator'. This increase the complexity from O(n) to O(log n).
    Therefore, iterators are handle differently depending on their category.
  */
  template <typename InputIterator>
  vector(typename enable_if<!is_arithmetic<InputIterator>::value,
                            InputIterator>::type first,
         InputIterator last, allocator_type const &alloc = allocator_type())
      : _t_vector_base(alloc) {
    typedef typename iterator_traits<InputIterator>::iterator_category
        _t_iterator_category;
    _range_initialize(first, last, _t_iterator_category());
  }

  /* ------------------------------ destructor ------------------------------ */
  ~vector() {
    destroy_a(this->_m_impl._m_start, this->_m_impl._m_finish,
              this->get_allocator());
  }

  /* -------------------------------- assign -------------------------------- */
  vector &operator=(vector const &x) {
    if (&x != this) {
      size_type const len = x.size();
      if (len > capacity()) {
        pointer tmp = _allocate_and_copy(len, x.begin(), x.end());
        _destroy_deallocate_set(tmp, tmp + len,
                                this->_m_impl._m_end_of_storage);
      } else if (size() >= len) {
        iterator i(std::copy(x.begin(), x.end(), begin()));
        destroy_a(i, end(), this->get_allocator());
      } else {
        std::copy(x.begin(), x.begin() + size(), this->_m_impl._m_start);
        uninitialized_copy_a(x.begin() + size(), x.end(),
                             this->_m_impl._m_finish, this->get_allocator());
      }
      this->_m_impl._m_finish = this->_m_impl._m_start + len;
    }
    return *this;
  }

protected:
  template <typename _T_InputIterator>
  void _assign(_T_InputIterator first, _T_InputIterator last,
               std::input_iterator_tag) {
    iterator cur(begin());
    for (; first != last && cur != end(); ++cur, ++first) {
      *cur = *first;
    }
    if (first == last) {
      erase(cur, end());
    } else {
      insert(end(), first, last);
    }
  }

  template <typename _T_ForwardIterator>
  void _assign(_T_ForwardIterator first, _T_ForwardIterator last,
               std::forward_iterator_tag) {
    size_type const len = std::distance(first, last);
    if (len > capacity()) {
      pointer tmp(_allocate_and_copy(len, first, last));
      _destroy_deallocate_set(tmp, tmp + len, tmp + len);
    } else if (size() >= len) {
      iterator finish(std::copy(first, last, this->_m_impl._m_start));
      destroy_a(finish, end(), this->get_allocator());
      this->_m_impl._m_finish = finish.base();
    } else {
      _T_ForwardIterator max = first;
      std::advance(max, size());
      std::copy(first, max, this->_m_impl._m_start);
      this->_m_impl._m_finish = uninitialized_copy_a(
          max, last, this->_m_impl._m_finish, this->get_allocator());
    }
  }

public:
  void assign(size_type n, value_type const &val) {
    if (n > capacity()) {
      vector tmp(n, val, get_allocator());
      tmp.swap(*this);
    } else if (n > size()) {
      std::fill(begin(), end(), val);
      uninitialized_fill_n_a(this->_m_impl._m_finish, n - size(), val,
                             this->get_allocator());
      this->_m_impl._m_finish += n - size();
    } else {
      erase(fill_n(begin(), n, val), end());
    }
  }

  // see comment on range constructor
  template <typename InputIterator>
  void assign(typename enable_if<!is_arithmetic<InputIterator>::value,
                                 InputIterator>::type first,
              InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category
        _t_iterator_category;
    _assign(first, last, _t_iterator_category());
  }

  /* ------------------------------- allocator ------------------------------ */
  allocator_type get_allocator() const {
    return *static_cast<allocator_type const *>(&this->_m_impl);
  }

  /* ------------------------------- iterator ------------------------------- */
  iterator begin() { return iterator(this->_m_impl._m_start); }

  const_iterator begin() const {
    return const_iterator(this->_m_impl._m_start);
  }

  iterator end() { return iterator(this->_m_impl._m_finish); }

  const_iterator end() const { return const_iterator(this->_m_impl._m_finish); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  /* ------------------------------- capacity ------------------------------- */
  bool empty() const { return begin() == end(); }

  size_type size() const { return size_type(end() - begin()); }

  size_type max_size() const {
    return std::min(
        static_cast<size_type>(std::numeric_limits<difference_type>::max()),
        this->_m_impl.max_size());
  }

  void reserve(size_type n) {
    if (n > this->max_size()) {
      throw std::length_error("vector::reserve");
    }
    if (this->capacity() < n) {
      size_type const old_size = size();
      pointer tmp = _allocate_and_copy(n, this->_m_impl._m_start,
                                       this->_m_impl._m_finish);
      _destroy_deallocate_set(tmp, tmp + old_size, tmp + n);
    }
  }

  size_type capacity() const {
    return size_type(const_iterator(this->_m_impl._m_end_of_storage) - begin());
  }

  /* ------------------------------- modifiers ------------------------------ */
  void clear() { erase(begin(), end()); }

protected:
  void _insert(iterator position, value_type const &x) {
    if (this->_m_impl._m_finish != this->_m_impl._m_end_of_storage) {
      this->_m_impl.construct(this->_m_impl._m_finish,
                              *(this->_m_impl._m_finish - 1));
      ++this->_m_impl._m_finish;
      value_type x_copy = x;
      std::copy_backward(position, iterator(this->_m_impl._m_finish - 2),
                         iterator(this->_m_impl._m_finish - 1));
      *position = x_copy;
    } else {
      size_type const old_size = size();
      size_type len = old_size != 0 ? 2 * old_size : 1;
      if (len < old_size) {
        len = this->max_size();
      }
      iterator new_start(this->allocate(len));
      iterator new_finish(new_start);
      try {
        new_finish =
            uninitialized_copy_a(iterator(this->_m_impl._m_start), position,
                                 new_start, this->get_allocator());
        this->_m_impl.construct(new_finish.base(), x);
        ++new_finish;
        new_finish =
            uninitialized_copy_a(position, iterator(this->_m_impl._m_finish),
                                 new_finish, this->get_allocator());
      } catch (...) {
        destroy_a(new_start, new_finish, this->get_allocator());
        this->deallocate(new_start.base(), len);
        throw;
      }
      _destroy_deallocate_set(new_start.base(), new_finish.base(),
                              new_start.base() + len);
    }
  }

  template <typename _T_InputIterator>
  void _range_insert(iterator pos, _T_InputIterator first,
                     _T_InputIterator last, std::input_iterator_tag) {
    for (; first != last; ++first) {
      pos = insert(pos, *first);
      ++pos;
    }
  }

  template <typename _T_ForwardIterator>
  void _range_insert(iterator position, _T_ForwardIterator first,
                     _T_ForwardIterator last, std::forward_iterator_tag) {
    if (first != last) {
      size_type const n = std::distance(first, last);
      if (size_type(this->_m_impl._m_end_of_storage -
                    this->_m_impl._m_finish) >= n) {
        size_type const elems_after = end() - position;
        iterator old_finish(this->_m_impl._m_finish);
        if (elems_after > n) {
          uninitialized_copy_a(this->_m_impl._m_finish - n,
                               this->_m_impl._m_finish, this->_m_impl._m_finish,
                               this->get_allocator());
          this->_m_impl._m_finish += n;
          std::copy_backward(position, old_finish - n, old_finish);
          std::copy(first, last, position);
        } else {
          _T_ForwardIterator max = first;
          std::advance(max, elems_after);
          uninitialized_copy_a(max, last, this->_m_impl._m_finish,
                               this->get_allocator());
          this->_m_impl._m_finish += n - elems_after;
          uninitialized_copy_a(position, old_finish, this->_m_impl._m_finish,
                               this->get_allocator());
          this->_m_impl._m_finish += elems_after;
          std::copy(first, max, position);
        }
      } else {
        size_type const old_size = size();
        size_type len = old_size + std::max(old_size, n);
        if (len < old_size) {
          len = this->max_size();
        }
        iterator new_start(this->allocate(len));
        iterator new_finish(new_start);
        try {
          new_finish =
              uninitialized_copy_a(iterator(this->_m_impl._m_start), position,
                                   new_start, this->get_allocator());
          new_finish = uninitialized_copy_a(first, last, new_finish,
                                            this->get_allocator());
          new_finish =
              uninitialized_copy_a(position, iterator(this->_m_impl._m_finish),
                                   new_finish, this->get_allocator());
        } catch (...) {
          destroy_a(new_start, new_finish, this->get_allocator());
          this->deallocate(new_start.base(), len);
          throw;
        }
        _destroy_deallocate_set(new_start.base(), new_finish.base(),
                                new_start.base() + len);
      }
    }
  }

public:
  iterator insert(iterator position, value_type const &val) {
    size_type const n = position - begin();
    if (this->_m_impl._m_finish != this->_m_impl._m_end_of_storage &&
        position == end()) {
      this->_m_impl.construct(this->_m_impl._m_finish, val);
      ++this->_m_impl._m_finish;
    } else
      _insert(position, val);
    return begin() + n;
  }

  void insert(iterator position, size_type n, value_type const &val) {
    if (n != 0) {
      if (size_type(this->_m_impl._m_end_of_storage -
                    this->_m_impl._m_finish) >= n) {
        value_type x_copy = val;
        size_type const elems_after = end() - position;
        iterator old_finish(this->_m_impl._m_finish);
        if (elems_after > n) {
          uninitialized_copy_a(this->_m_impl._m_finish - n,
                               this->_m_impl._m_finish, this->_m_impl._m_finish,
                               this->get_allocator());
          this->_m_impl._m_finish += n;
          std::copy_backward(position, old_finish - n, old_finish);
          std::fill(position, position + n, x_copy);
        } else {
          uninitialized_fill_n_a(this->_m_impl._m_finish, n - elems_after,
                                 x_copy, this->get_allocator());
          this->_m_impl._m_finish += n - elems_after;
          uninitialized_copy_a(position, old_finish, this->_m_impl._m_finish,
                               this->get_allocator());
          this->_m_impl._m_finish += elems_after;
          std::fill(position, old_finish, x_copy);
        }
      } else {
        size_type const old_size = size();
        size_type len = old_size + std::max(old_size, n);
        if (len < old_size) {
          len = this->max_size();
        }
        iterator new_start(this->allocate(len));
        iterator new_finish(new_start);
        try {
          new_finish = uninitialized_copy_a(begin(), position, new_start,
                                            this->get_allocator());
          uninitialized_fill_n_a(new_finish, n, val, this->get_allocator());
          new_finish += n;
          new_finish = uninitialized_copy_a(position, end(), new_finish,
                                            this->get_allocator());
        } catch (...) {
          destroy_a(new_start, new_finish, this->get_allocator());
          this->deallocate(new_start.base(), len);
          throw;
        }
        _destroy_deallocate_set(new_start.base(), new_finish.base(),
                                new_start.base() + len);
      }
    }
  }

  // see comment on range constructor
  template <typename InputIterator>
  void insert(iterator pos,
              typename enable_if<!is_arithmetic<InputIterator>::value,
                                 InputIterator>::type first,
              InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category
        _t_iterator_category;
    _range_insert(pos, first, last, _t_iterator_category());
  }

  iterator erase(iterator position) {
    if (position + 1 != end()) {
      std::copy(position + 1, end(), position);
    }
    --this->_m_impl._m_finish;
    this->_m_impl.destroy(this->_m_impl._m_finish);
    return position;
  }

  iterator erase(iterator first, iterator last) {
    iterator i(std::copy(last, end(), first));
    destroy_a(i, end(), this->get_allocator());
    this->_m_impl._m_finish =
        this->_m_impl._m_finish - std::distance(first, last);
    return first;
  }

  void push_back(value_type const &value) {
    if (this->_m_impl._m_finish != this->_m_impl._m_end_of_storage) {
      this->_m_impl.construct(this->_m_impl._m_finish, value);
      ++this->_m_impl._m_finish;
    } else
      _insert(end(), value);
  }

  void pop_back() {
    --this->_m_impl._m_finish;
    this->_m_impl.destroy(this->_m_impl._m_finish);
  }

  void resize(size_type count, value_type const &value) {
    if (count < size()) {
      erase(begin() + count, end());
    } else {
      insert(end(), count - size(), value);
    }
  }

  void resize(size_type count) { resize(count, value_type()); }

  void swap(_t_self &other) {
    std::swap(this->_m_impl._m_start, other._m_impl._m_start);
    std::swap(this->_m_impl._m_finish, other._m_impl._m_finish);
    std::swap(this->_m_impl._m_end_of_storage, other._m_impl._m_end_of_storage);
  }

  /* ---------------------------- element access ---------------------------- */
  reference operator[](size_type pos) { return *(begin() + pos); }

  const_reference operator[](size_type pos) const { return *(begin() + pos); }

  reference at(size_type pos) {
    if (pos >= this->size()) {
      throw std::out_of_range("vector::at");
    }
    return (*this)[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= this->size()) {
      throw std::out_of_range("vector::at");
    }
    return (*this)[pos];
  }

  reference front() { return *begin(); }

  const_reference front() const { return *begin(); }

  reference back() { return *(end() - 1); }

  const_reference back() const { return *(end() - 1); }

  /* -------------------------------- private ------------------------------- */
private:
  template <typename _T_ForwardIterator>
  pointer _allocate_and_copy(size_type n, _T_ForwardIterator first,
                             _T_ForwardIterator last) {
    pointer result = this->allocate(n);
    try {
      uninitialized_copy_a(first, last, result, this->get_allocator());
      return result;
    } catch (...) {
      this->deallocate(result, n);
      throw;
    }
  }

  void _destroy_deallocate_set(pointer new_start, pointer new_finish,
                               pointer end_of_storage) {
    destroy_a(begin(), end(), this->get_allocator());
    this->deallocate(this->_m_impl._m_start,
                     this->_m_impl._m_end_of_storage - this->_m_impl._m_start);
    this->_m_impl._m_start = new_start;
    this->_m_impl._m_finish = new_finish;
    this->_m_impl._m_end_of_storage = end_of_storage;
  }
};

/* -------------------------- non-member functions -------------------------- */
template <typename _T_Value, typename _T_Allocator>
inline void swap(vector<_T_Value, _T_Allocator> &lhs,
                 vector<_T_Value, _T_Allocator> &rhs) {
  lhs.swap(rhs);
}

template <typename _T_Value, typename _T_Allocator>
inline bool operator==(vector<_T_Value, _T_Allocator> const &lhs,
                       vector<_T_Value, _T_Allocator> const &rhs) {
  return (lhs.size() == rhs.size() &&
          equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename _T_Value, typename _T_Allocator>
inline bool operator<(vector<_T_Value, _T_Allocator> const &lhs,
                      vector<_T_Value, _T_Allocator> const &rhs) {
  return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                 rhs.end());
}

template <typename _T_Value, typename _T_Allocator>
inline bool operator!=(vector<_T_Value, _T_Allocator> const &lhs,
                       vector<_T_Value, _T_Allocator> const &rhs) {
  return !(lhs == rhs);
}

template <typename _T_Value, typename _T_Allocator>
inline bool operator>(vector<_T_Value, _T_Allocator> const &lhs,
                      vector<_T_Value, _T_Allocator> const &rhs) {
  return rhs < lhs;
}

template <typename _T_Value, typename _T_Allocator>
inline bool operator<=(vector<_T_Value, _T_Allocator> const &lhs,
                       vector<_T_Value, _T_Allocator> const &rhs) {
  return !(rhs < lhs);
}

template <typename _T_Value, typename _T_Allocator>
inline bool operator>=(vector<_T_Value, _T_Allocator> const &lhs,
                       vector<_T_Value, _T_Allocator> const &rhs) {
  return !(lhs < rhs);
}
} // namespace ft

#endif
