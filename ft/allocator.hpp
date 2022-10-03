#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

namespace ft {
template <typename _T_ForwardIterator, typename _T_Allocator>
void destroy_a(_T_ForwardIterator first, _T_ForwardIterator last,
               _T_Allocator alloc) {
  for (; first != last; ++first) {
    alloc.destroy(&*first);
  }
}

template <typename _T_ForwardIterator, typename _T_Size, typename _T_Value,
          typename _T_Allocator>
void uninitialized_fill_n_a(_T_ForwardIterator first, _T_Size n,
                            _T_Value const &x, _T_Allocator alloc) {
  _T_ForwardIterator cur = first;
  try {
    for (; n > 0; --n, ++cur) {
      alloc.construct(&*cur, x);
    }
  } catch (...) {
    destroy_a(first, cur, alloc);
    throw;
  }
}

template <typename _T_InputIterator, typename _T_ForwardIterator,
          typename _T_Allocator>
_T_ForwardIterator
uninitialized_copy_a(_T_InputIterator first, _T_InputIterator last,
                     _T_ForwardIterator result, _T_Allocator alloc) {
  _T_ForwardIterator cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      alloc.construct(&*cur, *first);
    }
    return cur;
  } catch (...) {
    destroy_a(result, cur, alloc);
    throw;
  }
}
} // namespace ft

#endif
