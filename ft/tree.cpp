#include "tree.hpp"

namespace ft {
Rb_tree_node_base *Rb_tree_node_base::min(Rb_tree_node_base *x) {
  while (x->_m_left != NULL) {
    x = x->_m_left;
  }
  return x;
}

Rb_tree_node_base const *Rb_tree_node_base::min(Rb_tree_node_base const *x) {
  while (x->_m_left != NULL) {
    x = x->_m_left;
  }
  return x;
}

Rb_tree_node_base *Rb_tree_node_base::max(Rb_tree_node_base *x) {
  while (x->_m_right != NULL) {
    x = x->_m_right;
  }
  return x;
}

Rb_tree_node_base const *Rb_tree_node_base::max(Rb_tree_node_base const *x) {
  while (x->_m_right != NULL) {
    x = x->_m_right;
  }
  return x;
}

void Rb_tree_rotate_left(Rb_tree_node_base *const x, Rb_tree_node_base *&root) {
  Rb_tree_node_base *const y = x->_m_right;

  x->_m_right = y->_m_left;
  if (y->_m_left != NULL) {
    y->_m_left->_m_parent = x;
  }
  y->_m_parent = x->_m_parent;

  if (x == root) {
    root = y;
  } else if (x == x->_m_parent->_m_left) {
    x->_m_parent->_m_left = y;
  } else {
    x->_m_parent->_m_right = y;
  }
  y->_m_left = x;
  x->_m_parent = y;
}

void Rb_tree_rotate_right(Rb_tree_node_base *const x,
                          Rb_tree_node_base *&root) {
  Rb_tree_node_base *const y = x->_m_left;

  x->_m_left = y->_m_right;
  if (y->_m_right != NULL) {
    y->_m_right->_m_parent = x;
  }
  y->_m_parent = x->_m_parent;

  if (x == root) {
    root = y;
  } else if (x == x->_m_parent->_m_right) {
    x->_m_parent->_m_right = y;
  } else {
    x->_m_parent->_m_left = y;
  }
  y->_m_right = x;
  x->_m_parent = y;
}

void Rb_tree_insert_and_rebalance(bool const insert_left, Rb_tree_node_base *x,
                                  Rb_tree_node_base *p,
                                  Rb_tree_node_base &header) {
  Rb_tree_node_base *&root = header._m_parent;

  x->_m_parent = p;
  x->_m_left = NULL;
  x->_m_right = NULL;
  x->_m_color = RBT_RED;

  if (insert_left) {
    p->_m_left = x;
    if (p == &header) {
      header._m_parent = x;
      header._m_right = x;
    } else if (p == header._m_left)
      header._m_left = x;
  } else {
    p->_m_right = x;
    if (p == header._m_right) {
      header._m_right = x;
    }
  }
  while (x != root && x->_m_parent->_m_color == RBT_RED) {
    Rb_tree_node_base *const xpp = x->_m_parent->_m_parent;

    if (x->_m_parent == xpp->_m_left) {
      Rb_tree_node_base *const y = xpp->_m_right;
      if (y && y->_m_color == RBT_RED) {
        x->_m_parent->_m_color = RBT_BLACK;
        y->_m_color = RBT_BLACK;
        xpp->_m_color = RBT_RED;
        x = xpp;
      } else {
        if (x == x->_m_parent->_m_right) {
          x = x->_m_parent;
          Rb_tree_rotate_left(x, root);
        }
        x->_m_parent->_m_color = RBT_BLACK;
        xpp->_m_color = RBT_RED;
        Rb_tree_rotate_right(xpp, root);
      }
    } else {
      Rb_tree_node_base *const y = xpp->_m_left;
      if (y && y->_m_color == RBT_RED) {
        x->_m_parent->_m_color = RBT_BLACK;
        y->_m_color = RBT_BLACK;
        xpp->_m_color = RBT_RED;
        x = xpp;
      } else {
        if (x == x->_m_parent->_m_left) {
          x = x->_m_parent;
          Rb_tree_rotate_right(x, root);
        }
        x->_m_parent->_m_color = RBT_BLACK;
        xpp->_m_color = RBT_RED;
        Rb_tree_rotate_left(xpp, root);
      }
    }
  }
  root->_m_color = RBT_BLACK;
}

Rb_tree_node_base *Rb_tree_rebalance_for_erase(Rb_tree_node_base *const z,
                                               Rb_tree_node_base &header) {
  Rb_tree_node_base *&root = header._m_parent;
  Rb_tree_node_base *&leftmost = header._m_left;
  Rb_tree_node_base *&rightmost = header._m_right;
  Rb_tree_node_base *y = z;
  Rb_tree_node_base *x = NULL;
  Rb_tree_node_base *x_parent = NULL;

  if (y->_m_left == NULL) {
    x = y->_m_right;
  } else if (y->_m_right == NULL) {
    x = y->_m_left;
  } else {
    y = y->_m_right;
    while (y->_m_left != NULL) {
      y = y->_m_left;
    }
    x = y->_m_right;
  }
  if (y != z) {
    z->_m_left->_m_parent = y;
    y->_m_left = z->_m_left;
    if (y != z->_m_right) {
      x_parent = y->_m_parent;
      if (x) {
        x->_m_parent = y->_m_parent;
      }
      y->_m_parent->_m_left = x;
      y->_m_right = z->_m_right;
      z->_m_right->_m_parent = y;
    } else {
      x_parent = y;
    }
    if (root == z) {
      root = y;
    } else if (z->_m_parent->_m_left == z) {
      z->_m_parent->_m_left = y;
    } else {
      z->_m_parent->_m_right = y;
    }
    y->_m_parent = z->_m_parent;
    std::swap(y->_m_color, z->_m_color);
    y = z;
  } else {
    x_parent = y->_m_parent;
    if (x) {
      x->_m_parent = y->_m_parent;
    }
    if (root == z) {
      root = x;
    } else if (z->_m_parent->_m_left == z) {
      z->_m_parent->_m_left = x;
    } else {
      z->_m_parent->_m_right = x;
    }
    if (leftmost == z) {
      if (z->_m_right == NULL) {
        leftmost = z->_m_parent;
      } else {
        leftmost = Rb_tree_node_base::min(x);
      }
    }
    if (rightmost == z) {
      if (z->_m_left == NULL) {
        rightmost = z->_m_parent;
      } else {
        rightmost = Rb_tree_node_base::max(x);
      }
    }
  }
  if (y->_m_color != RBT_RED) {
    while (x != root && (x == NULL || x->_m_color == RBT_BLACK)) {
      if (x == x_parent->_m_left) {
        Rb_tree_node_base *w = x_parent->_m_right;
        if (w->_m_color == RBT_RED) {
          w->_m_color = RBT_BLACK;
          x_parent->_m_color = RBT_RED;
          Rb_tree_rotate_left(x_parent, root);
          w = x_parent->_m_right;
        }
        if ((w->_m_left == NULL || w->_m_left->_m_color == RBT_BLACK) &&
            (w->_m_right == NULL || w->_m_right->_m_color == RBT_BLACK)) {
          w->_m_color = RBT_RED;
          x = x_parent;
          x_parent = x_parent->_m_parent;
        } else {
          if (w->_m_right == NULL || w->_m_right->_m_color == RBT_BLACK) {
            w->_m_left->_m_color = RBT_BLACK;
            w->_m_color = RBT_RED;
            Rb_tree_rotate_right(w, root);
            w = x_parent->_m_right;
          }
          w->_m_color = x_parent->_m_color;
          x_parent->_m_color = RBT_BLACK;
          if (w->_m_right) {
            w->_m_right->_m_color = RBT_BLACK;
          }
          Rb_tree_rotate_left(x_parent, root);
          break;
        }
      } else {
        Rb_tree_node_base *w = x_parent->_m_left;
        if (w->_m_color == RBT_RED) {
          w->_m_color = RBT_BLACK;
          x_parent->_m_color = RBT_RED;
          Rb_tree_rotate_right(x_parent, root);
          w = x_parent->_m_left;
        }
        if ((w->_m_right == NULL || w->_m_right->_m_color == RBT_BLACK) &&
            (w->_m_left == NULL || w->_m_left->_m_color == RBT_BLACK)) {
          w->_m_color = RBT_RED;
          x = x_parent;
          x_parent = x_parent->_m_parent;
        } else {
          if (w->_m_left == NULL || w->_m_left->_m_color == RBT_BLACK) {
            w->_m_right->_m_color = RBT_BLACK;
            w->_m_color = RBT_RED;
            Rb_tree_rotate_left(w, root);
            w = x_parent->_m_left;
          }
          w->_m_color = x_parent->_m_color;
          x_parent->_m_color = RBT_BLACK;
          if (w->_m_left) {
            w->_m_left->_m_color = RBT_BLACK;
          }
          Rb_tree_rotate_right(x_parent, root);
          break;
        }
      }
    }
    if (x) {
      x->_m_color = RBT_BLACK;
    }
  }
  return y;
}

Rb_tree_node_base *Rb_tree_node_increment(Rb_tree_node_base *node) {
  if (node->_m_right != NULL) {
    node = node->_m_right;
    while (node->_m_left != NULL) {
      node = node->_m_left;
    }
  } else {
    Rb_tree_node_base *y = node->_m_parent;
    while (node == y->_m_right) {
      node = y;
      y = y->_m_parent;
    }
    if (node->_m_right != y)
      node = y;
  }
  return node;
}

Rb_tree_node_base const *Rb_tree_node_increment(Rb_tree_node_base const *node) {
  return Rb_tree_node_increment(const_cast<Rb_tree_node_base *>(node));
}

Rb_tree_node_base *Rb_tree_node_decrement(Rb_tree_node_base *node) {
  if (node->_m_color == RBT_RED && node->_m_parent->_m_parent == node)
    node = node->_m_right;
  else if (node->_m_left != NULL) {
    Rb_tree_node_base *y = node->_m_left;
    while (y->_m_right != NULL) {
      y = y->_m_right;
    }
    node = y;
  } else {
    Rb_tree_node_base *y = node->_m_parent;
    while (node == y->_m_left) {
      node = y;
      y = y->_m_parent;
    }
    node = y;
  }
  return node;
}

Rb_tree_node_base const *Rb_tree_node_decrement(Rb_tree_node_base const *node) {
  return Rb_tree_node_decrement(const_cast<Rb_tree_node_base *>(node));
}
} // namespace ft
