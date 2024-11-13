#pragma once
#include <cstdlib>
#include <stddef.h>
#include <memory>
#include <iostream>
#include <type_traits>
#include <iterator>
#include <limits>
#include <utility>
#include "tree.hpp"

enum class tag : int {inorder_tag, preorder_tag, postorder_tag};

template<typename T, typename Allocator = std::allocator<Node<T>>>
class BinarySearchTree {
public:

  typedef T& reference;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef T value_type;
  typedef Node<T> node_type;

  BinarySearchTree() : size_(0) {}

  BinarySearchTree(BinarySearchTree& other) : tree_(new BST<value_type, Allocator>(*other.tree_)), size_(other.size_) {}

  BinarySearchTree(BinarySearchTree&& other) noexcept : tree_(other.tree_), size_(other.size_) { other.tree_ = nullptr; other.size_ = 0; }

  BinarySearchTree(const std::initializer_list<value_type>& il);

  BinarySearchTree(std::initializer_list<value_type>&& il);

  ~BinarySearchTree() { if (size_ > 0) tree_->DestroyTree(*tree_->Top()); }

  BinarySearchTree& InitializeBSTobject() { 
    this->tree_ = new BST<value_type, Allocator>;
    return *this;
  }

  template<tag Tag>
  class iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    iterator() : ptr_(nullptr) {}

    explicit iterator(node_type* ptr) : ptr_(ptr) {}
    reference operator*() { 
      if (ptr_) {
        value_type* temp = const_cast<value_type*>(&ptr_->value_);
        return *temp;
      } else {
        throw std::runtime_error("Dereferencing a null pointer");
      }
      }
    iterator& operator++();
    iterator& operator++(int) {  iterator<Tag> temp = *this; ++(*this); return temp; }

    iterator& operator--();
    iterator& operator--(int) { iterator<Tag> temp = *this; --(*this); return temp; }
    friend bool operator==(const iterator& a, const iterator& b) { return a.ptr_ == b.ptr_; }
    friend bool operator!=(const iterator& a, const iterator& b) { return a.ptr_ != b.ptr_; }

   private:
    node_type* ptr_;
  };

  template<tag Tag>
  class const_iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = ptrdiff_t;
    using value_type = const T;
    using pointer = const T*;
    using const_reference = const T&;

    const_iterator() : ptr_(nullptr) {}

    explicit const_iterator(node_type* ptr) : ptr_(ptr) {}
    const_reference operator*() const { 
      if (ptr_) {
        value_type* temp = const_cast<value_type*>(&ptr_->value_);
        return *temp;
      } else {
        throw std::runtime_error("Dereferencing a null pointer");
      }
    }
    const_iterator& operator++();
    const_iterator& operator++(int) {  const_iterator<Tag> temp = *this; ++(*this); return temp; }

    const_iterator& operator--();
    const_iterator& operator--(int) { iterator<Tag> temp = *this; --(*this); return temp; }

    friend bool operator==(const const_iterator& a, const const_iterator& b) { return a.ptr_ == b.ptr_; }
    friend bool operator!=(const const_iterator& a, const const_iterator& b) { return a.ptr_ != b.ptr_; }

   private:
    node_type* ptr_;
  };

  template<tag Tag>
  class reverse_iterator {
  public:
    reverse_iterator() = default;

    explicit constexpr reverse_iterator(iterator<Tag> itr) : it_(itr) {}
    constexpr decltype(auto) operator*() const { 
      return *std::prev(it_);
    }
    constexpr reverse_iterator& operator++() { --it_; return *this; }
    constexpr reverse_iterator& operator++(int) { auto temp = *this; ++(*this); return temp; }

    constexpr reverse_iterator& operator--() { ++it_; return *this; }

    constexpr reverse_iterator& operator--(int) {
      auto temp = *this;
      --(*this);
      return temp;
    }

    friend bool operator==(const reverse_iterator& a, const reverse_iterator& b) { return a.it_ == b.it_; }
    friend bool operator!=(const reverse_iterator& a, const reverse_iterator& b) { return a.it_ != b.it_; }

   private:
    iterator<Tag> it_ = iterator<Tag>();
  };

  template<tag Tag>
  class const_reverse_iterator {
  public:
    const_reverse_iterator() = default;

    explicit constexpr const_reverse_iterator(const_iterator<Tag> itr) : it_(itr) {}
    constexpr decltype(auto) operator*() const { 
      return *std::prev(it_);
    }
    constexpr const_reverse_iterator& operator++() { --it_; return *this; }
    constexpr const_reverse_iterator& operator++(int) { auto temp = *this; ++(*this); return temp; }

    constexpr const_reverse_iterator& operator--() { ++it_; return *this; }
    constexpr const_reverse_iterator& operator--(int) { auto temp = *this; --(*this); return temp; }

    friend bool operator==(const const_reverse_iterator& a, const const_reverse_iterator& b) { return a.it_ == b.it_; }
    friend bool operator!=(const const_reverse_iterator& a, const const_reverse_iterator& b) { return a.it_ != b.it_; }

   private:
    const_iterator<Tag> it_ = const_iterator<Tag>();
  };

  template<tag Tag>
  using iterator_type = typename BinarySearchTree<T, Allocator>::template iterator<Tag>;
  template<tag Tag>
  using const_iterator_type = const typename BinarySearchTree<T, Allocator>::template const_iterator<Tag>;

  template<tag Tag>
  iterator_type<Tag> begin();

  template<tag Tag>
  reverse_iterator<Tag> rbegin();

  template<tag Tag>
  const_iterator_type<Tag> cbegin() const;

  template<tag Tag>
  const_reverse_iterator<Tag> crbegin() const;

  template<tag Tag>
  iterator_type<Tag> end();

  template<tag Tag>
  reverse_iterator<Tag> rend();
  
  template<tag Tag>
  const_iterator_type<Tag> cend() const;

  template<tag Tag>
  const_reverse_iterator<Tag> crend() const;
 
  std::pair<iterator_type<tag::preorder_tag>, bool> insert(const value_type& value);
  std::pair<iterator_type<tag::preorder_tag>, bool> insert(value_type&& value);
  template<typename InputIt>
  void insert(InputIt first, InputIt last);
  void insert(std::initializer_list<value_type> ilist);
  
  size_type erase(const value_type& value);
  template<tag Tag>
  const_iterator_type<Tag> erase(const_iterator_type<Tag> it);
  template <tag Tag>
  iterator_type<Tag> erase(iterator_type<Tag> it);

  node_type extract(const value_type& value);
  template<tag Tag>
  node_type extract(const_iterator_type<Tag> it);

  template<tag Tag>
  iterator_type<Tag> find(const value_type& value);
  
  BinarySearchTree& operator= (BinarySearchTree&& other);
  BinarySearchTree& operator= (const BinarySearchTree& other);
  BinarySearchTree& operator= (const std::initializer_list<value_type>& il);
  BinarySearchTree& operator= (std::initializer_list<value_type>&& il);

  friend bool operator==(const BinarySearchTree& tree, const BinarySearchTree& tree2) { return tree.tree_->EqualTrees(*tree.tree_->Top(), *tree2.tree_->Top()); }

  friend bool operator!=(const BinarySearchTree& tree,const BinarySearchTree& tree2) { return !operator==(tree, tree2); }

  void swap(BinarySearchTree& other) noexcept;

  bool empty() const { return size_ == 0; }

  void clear() noexcept;
  size_t size() const { return size_; }
  size_t max_size() const { return std::numeric_limits<value_type>::max() / sizeof(value_type); }
  
  template<tag Tag, typename... Args>
  std::pair<iterator<Tag>, bool> emplace(Args&&... args);
  
  void merge(BinarySearchTree& other);

  bool contains(const value_type& value);

  template<tag Tag>
  iterator_type<Tag> upper_bound(const value_type& value);

  template<tag Tag>
  iterator_type<Tag> lower_bound(const value_type& value);

private:  
  Tree<value_type>* tree_;
  size_t size_;
  mutable Allocator allocator_;
};

template <typename T, typename Allocator>
BinarySearchTree<T, Allocator>::BinarySearchTree(const std::initializer_list<value_type>& il) {
  clear();
  insert(il);
}

template <typename T, typename Allocator>
BinarySearchTree<T, Allocator>::BinarySearchTree(std::initializer_list<value_type>&& il) {
  clear();
  insert(il);
}


template<typename T, typename Allocator>
template<tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator_type<Tag> BinarySearchTree<T, Allocator>::begin() {
  node_type* b_node = *tree_->Top();
  if constexpr (Tag == tag::inorder_tag) {
    while (b_node->left != nullptr) { 
      b_node = b_node->left;
    }
  }
  if constexpr (Tag == tag::preorder_tag) {
    b_node = *tree_->Top();
  }
  if constexpr (Tag == tag::postorder_tag) {
    while (b_node->left != nullptr) { 
      b_node = b_node->left;
    }
    while (b_node->right != nullptr) {
      b_node = b_node->right;
    }
  }
  return iterator_type<Tag>(b_node);
}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template reverse_iterator<Tag> BinarySearchTree<T, Allocator>::rbegin() {
  node_type* b_node = *tree_->Top();
  if constexpr (Tag == tag::inorder_tag) {
    while (b_node->right != nullptr) { b_node = b_node->right; }
    node_type* buffer = allocator_.allocate(1);
    buffer->parent = b_node;
    buffer->left = nullptr;
    buffer->right = nullptr;
    b_node->right = buffer;
    b_node = b_node->right;
  }
  if constexpr (Tag == tag::preorder_tag) {
    while (b_node->right != nullptr || b_node->left != nullptr) { 
      if (b_node->right) b_node = b_node->right; 
      else b_node = b_node->left;
    }
    node_type* buffer = allocator_.allocate(1);
    buffer->parent = b_node;
    buffer->left = nullptr;
    buffer->right = nullptr;
    b_node->right = buffer;
    b_node = b_node->right;
  }
  if (Tag == tag::postorder_tag) {
    node_type* buffer = allocator_.allocate(1);
    buffer->left = b_node;
    b_node = buffer;
  }

  return reverse_iterator<Tag>(iterator_type<Tag>(b_node));
}

template<typename T, typename Allocator>
template<tag Tag>
typename BinarySearchTree<T, Allocator>::template const_iterator_type<Tag> BinarySearchTree<T, Allocator>::cbegin() const{
  node_type* b_node = *tree_->cTop();
  if constexpr (Tag == tag::inorder_tag) {
    while (b_node->left != nullptr) { 
      b_node = b_node->left;
    }
  }
  if constexpr (Tag == tag::preorder_tag) {
    b_node = *tree_->cTop();
  }
  if constexpr (Tag == tag::postorder_tag) {
    while (b_node->left != nullptr) { 
      b_node = b_node->left;
    }
    while (b_node->right != nullptr) {
      b_node = b_node->right;
    }
  }
  return const_iterator_type<Tag>(b_node);
}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template const_reverse_iterator<Tag> BinarySearchTree<T, Allocator>::crbegin() const {
  node_type* b_node = *tree_->cTop();
  if constexpr (Tag == tag::inorder_tag) {
    while (b_node->right != nullptr) { b_node = b_node->right; }
    node_type* buffer = allocator_.allocate(1);
    buffer->parent = b_node;
    buffer->left = nullptr;
    buffer->right = nullptr;
    b_node->right = buffer;
    b_node = b_node->right;
  }
  if constexpr (Tag == tag::preorder_tag) {
    while (b_node->right != nullptr || b_node->left != nullptr) { 
      if (b_node->right != nullptr) b_node = b_node->right; 
      else b_node = b_node->left;
    }
    node_type* buffer = allocator_.allocate(1);
    buffer->parent = b_node;
    buffer->left = nullptr;
    buffer->right = nullptr;
    b_node->right = buffer;
    b_node = b_node->right;
  }
  if (Tag == tag::postorder_tag) {
    node_type* buffer = allocator_.allocate(1);
    buffer->left = b_node;
    b_node = buffer;
  }

  return const_reverse_iterator<Tag>(const_iterator_type<Tag>(b_node));
}

template<typename T, typename Allocator>
template<tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator_type<Tag> BinarySearchTree<T, Allocator>::end() {
  return iterator_type<Tag>(nullptr);
}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template reverse_iterator<Tag> BinarySearchTree<T, Allocator>::rend() {
  node_type* b_node = *tree_->Top();
  if constexpr (Tag == tag::inorder_tag) {
    while (b_node->left != nullptr) { 
      b_node = b_node->left;
    }
  }
  if constexpr (Tag == tag::preorder_tag) {
    b_node = *tree_->Top();
  }
  if constexpr (Tag == tag::postorder_tag) {
    while (b_node->left != nullptr || b_node->right != nullptr) {
     if (b_node->left) b_node = b_node->left;
     else b_node = b_node->right;
    }
  }
  return reverse_iterator<Tag>(iterator_type<Tag>(b_node));
}

template<typename T, typename Allocator>
template<tag Tag>
typename BinarySearchTree<T, Allocator>::template const_iterator_type<Tag> BinarySearchTree<T, Allocator>::cend() const{
  return const_iterator_type<Tag>(nullptr);
}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template const_reverse_iterator<Tag> BinarySearchTree<T, Allocator>::crend() const {
  node_type* b_node = *tree_->cTop();
  if constexpr (Tag == tag::inorder_tag) {
    while (b_node->left != nullptr) { 
      b_node = b_node->left;
    }
  }
  if constexpr (Tag == tag::preorder_tag) {
    b_node = *tree_->cTop();
  }
  if constexpr (Tag == tag::postorder_tag) {
    while (b_node->left != nullptr) {
      b_node = b_node->left;
    }
  }
  return const_reverse_iterator<Tag>(const_iterator_type<Tag>(b_node));
}

template<typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator<Tag>& BinarySearchTree<T, Allocator>::iterator<Tag>::operator++() {
    if constexpr (Tag == tag::inorder_tag) {
      if (ptr_->right != nullptr) {
        ptr_ = ptr_->right;
        while (ptr_->left != nullptr) {
          ptr_ = ptr_->left;
          
        }
      } else if (ptr_->parent != nullptr) {
        if (ptr_ == ptr_->parent->left) {
          ptr_ = ptr_->parent;
        } else {
          while (ptr_->parent != nullptr) {
            ptr_ = ptr_->parent;

            if(ptr_->parent == nullptr) {
              ptr_ = nullptr;
              return *this;
            }

            if (ptr_ == ptr_->parent->left) {
              ptr_ = ptr_->parent;
              break;
            }
          }
        }
      } else {
        ptr_ = nullptr;
      }
    }
    if constexpr (Tag == tag::preorder_tag) {
      if (ptr_ == nullptr) {
        return *this;
      }

      if (ptr_->left) {
        ptr_ = ptr_->left;
      } else if (ptr_->right) {
        ptr_ = ptr_->right;
      } else {
        while (ptr_->parent != nullptr) {
          if (ptr_ == ptr_->parent->left && ptr_->parent->right != nullptr) {
            ptr_ = ptr_->parent->right;
            break;
          }
          ptr_ = ptr_->parent;
        }
        
        if (ptr_->parent == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
        if (ptr_ != nullptr && ptr_->parent == nullptr && ptr_->right == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
      }
    }
    if constexpr (Tag == tag::postorder_tag) {
      if (ptr_->parent == nullptr) {
        ptr_ = nullptr;
      } else {
        if (ptr_ == ptr_->parent->right) {
        ptr_ = ptr_->parent;
        } else {
          if (ptr_->parent->right != nullptr) {
            ptr_ = ptr_->parent->right;
            while (ptr_->left != nullptr || ptr_->right != nullptr) {
              if (ptr_->left != nullptr)  ptr_ = ptr_->left;
              if (ptr_->right != nullptr) ptr_ = ptr_->right;
            }
          } else {
            ptr_ = ptr_->parent;
          }
        }
      }
    }
  return *this;
}

template<typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator<Tag>& BinarySearchTree<T, Allocator>::iterator<Tag>::operator--() {
  if constexpr (Tag == tag::inorder_tag) {
    if (ptr_->left != nullptr) {
        ptr_ = ptr_->left;
        while (ptr_->right != nullptr) {
            ptr_ = ptr_->right;
        }
    } else {
        while (ptr_->parent != nullptr && ptr_ == ptr_->parent->left) {
            ptr_ = ptr_->parent;
        }
        ptr_ = ptr_->parent; 
    }
    
  }
  if constexpr (Tag == tag::preorder_tag) {
    if (ptr_->parent->left != nullptr && ptr_ != ptr_->parent->left) {
      ptr_ = ptr_->parent->left;
      while (ptr_->right != nullptr || ptr_->left != nullptr) {
        if (ptr_->right != nullptr) ptr_ = ptr_->right;
        else ptr_ = ptr_->left;
      }
    } else {
      ptr_ = ptr_->parent;
    }
  }
  if constexpr (Tag == tag::postorder_tag) {    
    if (ptr_ == nullptr) {
        return *this;
      }

      if (ptr_->right) {
        ptr_ = ptr_->right;
      } else if (ptr_->left) {
        ptr_ = ptr_->left;
      } else {
        while (ptr_->parent != nullptr) {
          if (ptr_ == ptr_->parent->right && ptr_->parent->left != nullptr) {
            ptr_ = ptr_->parent->left;
            break;
          }
          ptr_ = ptr_->parent;
        }
        
        if (ptr_->parent == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
        if (ptr_ != nullptr && ptr_->parent == nullptr && ptr_->left == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
      }
    
  }
  return *this;
}

template<typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template const_iterator<Tag>& BinarySearchTree<T, Allocator>::const_iterator<Tag>::operator++() {
    if constexpr (Tag == tag::inorder_tag) {
      if (ptr_->right != nullptr) {
        ptr_ = ptr_->right;
        while (ptr_->left != nullptr) {
          ptr_ = ptr_->left;
          
        }
      } else if (ptr_->parent != nullptr) {
        if (ptr_ == ptr_->parent->left) {
          ptr_ = ptr_->parent;
        } else {
          while (ptr_->parent != nullptr) {
            ptr_ = ptr_->parent;

            if(ptr_->parent == nullptr) {
              ptr_ = nullptr;
              return *this;
            }

            if (ptr_ == ptr_->parent->left) {
              ptr_ = ptr_->parent;
              break;
            }
          }
        }
      } else {
        ptr_ = nullptr;
      }
    }
    if constexpr (Tag == tag::preorder_tag) {
      if (ptr_ == nullptr) {
        return *this;
      }

      if (ptr_->left) {
        ptr_ = ptr_->left;
      } else if (ptr_->right) {
        ptr_ = ptr_->right;
      } else {
        while (ptr_->parent != nullptr) {
          if (ptr_ == ptr_->parent->left && ptr_->parent->right != nullptr) {
            ptr_ = ptr_->parent->right;
            break;
          }
          ptr_ = ptr_->parent;
        }
        
        if (ptr_->parent == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
        if (ptr_ != nullptr && ptr_->parent == nullptr && ptr_->right == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
      }
    }
    if constexpr (Tag == tag::postorder_tag) {
      if (ptr_->parent == nullptr) {
        ptr_ = nullptr;
      } else {
        if (ptr_ == ptr_->parent->right) {
        ptr_ = ptr_->parent;
        } else {
          if (ptr_->parent->right != nullptr) {
            ptr_ = ptr_->parent->right;
            while (ptr_->left != nullptr || ptr_->right != nullptr) {
              if (ptr_->left != nullptr)  ptr_ = ptr_->left;
              if (ptr_->right != nullptr) ptr_ = ptr_->right;
            }
          } else {
            ptr_ = ptr_->parent;
          }
        }
      }
    }
  return *this;
}

template<typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template const_iterator<Tag>& BinarySearchTree<T, Allocator>::const_iterator<Tag>::operator--() {
   if constexpr (Tag == tag::inorder_tag) {
    if (ptr_->left != nullptr) {
        ptr_ = ptr_->left;
        while (ptr_->right != nullptr) {
            ptr_ = ptr_->right;
        }
    } else {
        while (ptr_->parent != nullptr && ptr_ == ptr_->parent->left) {
            ptr_ = ptr_->parent;
        }
        ptr_ = ptr_->parent; 
    }
    
  }
  if constexpr (Tag == tag::preorder_tag) {
    if (ptr_->parent->left != nullptr && ptr_ != ptr_->parent->left) {
      ptr_ = ptr_->parent->left;
      while (ptr_->right != nullptr || ptr_->left != nullptr) {
        if (ptr_->right != nullptr) ptr_ = ptr_->right;
        else ptr_ = ptr_->left;
      }
    } else {
      ptr_ = ptr_->parent;
    }
  }
  if constexpr (Tag == tag::postorder_tag) {    
    if (ptr_ == nullptr) {
        return *this;
      }

      if (ptr_->right) {
        ptr_ = ptr_->right;
      } else if (ptr_->left) {
        ptr_ = ptr_->left;
      } else {
        while (ptr_->parent != nullptr) {
          if (ptr_ == ptr_->parent->right && ptr_->parent->left != nullptr) {
            ptr_ = ptr_->parent->left;
            break;
          }
          ptr_ = ptr_->parent;
        }
        
        if (ptr_->parent == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
        if (ptr_ != nullptr && ptr_->parent == nullptr && ptr_->left == nullptr) {
          ptr_ = nullptr;
          return *this;
        }
      }
    
  }
  return *this;
}

template<typename T, typename Allocator>
std::pair<typename BinarySearchTree<T, Allocator>::template iterator_type<tag::preorder_tag>, bool> BinarySearchTree<T, Allocator>::insert(const value_type& value) {
  node_type* node = tree_->Find(value, *tree_->Top());
  if (node != nullptr) {
    return std::make_pair(iterator_type<tag::preorder_tag>(node), false);
  }
  node_type* new_node = allocator_.allocate(1);
  std::allocator_traits<Allocator>::construct(allocator_, new_node, value);
  ++size_;
  tree_->Insert(new_node, *tree_->Top());
  return std::make_pair(iterator_type<tag::preorder_tag>(new_node), true);
}

template <typename T, typename Allocator>
std::pair<typename BinarySearchTree<T, Allocator>::template iterator_type<tag::preorder_tag>, bool> BinarySearchTree<T, Allocator>::insert(value_type&& value) {
  node_type* node = tree_->Find(value, *tree_->Top());
  if (node != nullptr) {
    return std::make_pair(iterator_type<tag::preorder_tag>(node), false);
  }
  node_type* new_node = allocator_.allocate(1);
  std::allocator_traits<Allocator>::construct(allocator_, new_node, value);
  ++size_;
  tree_->Insert(new_node, *tree_->Top());
  return std::make_pair(iterator_type<tag::preorder_tag>(new_node), true);
  return {};
}

template <typename T, typename Allocator>
void BinarySearchTree<T, Allocator>::insert(std::initializer_list<value_type> ilist) {
  for (auto it = ilist.begin(); it != ilist.end(); ++it) {
    insert(*it);
  }
}

template <typename T, typename Allocator>
template <typename InputIt>
void BinarySearchTree<T, Allocator>::insert(InputIt first, InputIt last) {
  for (auto it = first; it != last; ++it) {
    insert(*it);
  }
}


template <typename T, typename Allocator>
size_t BinarySearchTree<T, Allocator>::erase(const value_type& value) {
  node_type* temp = tree_->Find(value, *tree_->Top());
  if (size_ > 0 && temp != nullptr) --size_;
  tree_->SetTop(tree_->Remove(*tree_->Top(), value));
  return temp == nullptr ? 0 : 1;
}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template const_iterator_type<Tag> BinarySearchTree<T, Allocator>::erase(const_iterator_type<Tag> it) {
  if (size_ > 0 && contains(*it)) --size_;
  auto temp = it;
  ++temp;
  tree_->SetTop(tree_->Remove(*tree_->Top(), *it));
  value_type t_v;
  try {
    t_v = *temp;
  }
  catch (...) {
    return const_iterator_type<Tag>(nullptr);
  }
  return const_iterator_type<Tag>(tree_->Find(t_v, *tree_->Top()));
}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator_type<Tag> BinarySearchTree<T, Allocator>::erase(iterator_type<Tag> it) {
  if (size_ > 0 && contains(*it)) --size_;
  auto temp = it;
  ++temp;
  tree_->SetTop(tree_->Remove(*tree_->Top(), *it));
  value_type t_v;
  try {
    t_v = *temp;
  }
  catch (...) {
    return iterator_type<Tag>(nullptr);
  }
  return iterator_type<Tag>(tree_->Find(t_v, *tree_->Top()));
}

template <typename T, typename Allocator>
Node<T> BinarySearchTree<T, Allocator>::extract(const value_type& value) {
  if (size_ > 0 && contains(value)) --size_;
  node_type* extracted = tree_->Extract(value);
  return *extracted;
}

template <typename T, typename Allocator>
template <tag Tag>
Node<T> BinarySearchTree<T, Allocator>::extract(const_iterator_type<Tag> it) {
  if (size_ > 0 && contains(*it)) --size_;
  node_type* extracted = tree_->Extract(*it);
  return *extracted;
}

template<typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator<Tag> BinarySearchTree<T, Allocator>::find(const value_type& value) {
  node_type* current = *tree_->Top();
  while (current != nullptr) {
    if (current->value_ == value) {
      return iterator_type<Tag>(current);
    }

    if (current->value_ < value) {
      current = current->right;
    } else if (current->value_ > value) {
      current = current->left;
    }
  }
  node_type* node = tree_->Find(value, *tree_->Top());
  if (node != nullptr) {
    return iterator<Tag>(node);
  }
  return end<Tag>();
}

template <typename T, typename Allocator>
BinarySearchTree<T, Allocator>& BinarySearchTree<T, Allocator>::operator=(const std::initializer_list<value_type>& il) {
  clear();
  insert(il);
}

template <typename T, typename Allocator>
BinarySearchTree<T, Allocator>& BinarySearchTree<T, Allocator>::operator=(std::initializer_list<value_type>&& il) {
  clear();
  insert(il);
}

template <typename T, typename Allocator>
BinarySearchTree<T, Allocator>& BinarySearchTree<T, Allocator>::operator=(
    BinarySearchTree&& other) {
  if (this != &other) {
    tree_->DestroyTree(tree_->Top());
    tree_->SetTop(tree_->DeepCopyTree(other.top));
  }
  return *this;
}

template <typename T, typename Allocator>
BinarySearchTree<T, Allocator>& BinarySearchTree<T, Allocator>::operator=(const BinarySearchTree& other) {
  this = other;
  return this;
}

template<typename T, typename Allocator>
void BinarySearchTree<T, Allocator>::swap(BinarySearchTree& other) noexcept{
  node_type* temp = tree_->Top();
  tree_->SetTop(other.tree_->Top());
  other.tree_->SetTop(temp);
}

template <typename T, typename Allocator>
void BinarySearchTree<T, Allocator>::clear() noexcept {
  size_ = 0;
  tree_->DestroyTree(*tree_->Top());
}

template <typename T, typename Allocator>
void BinarySearchTree<T, Allocator>::merge(BinarySearchTree<T, Allocator>& other) {
  for (auto it = other.begin<tag::preorder_tag>(); it != other.end<tag::preorder_tag>(); ++it) {
    this->insert(*it);
  }
  other.clear();
}

template <typename T, typename Allocator>
bool BinarySearchTree<T, Allocator>::contains(const value_type& value) {
  node_type* current = *tree_->Top();
  while (current != nullptr) {
    if (current->value_ == value) {
      return true;
    }

    if (current->value_ < value) {
      current = current->right;
    } else if (current->value_ > value) {
      current = current->left;
    }
  }
  return false;
}

template<typename T, typename Allocator>
void swap(BinarySearchTree<T, Allocator>& t1, BinarySearchTree<T, Allocator>& t2) {
  t1.swap(t2);
}

template<typename T, typename Allocator>
template<tag Tag, typename... Args>
std::pair<typename BinarySearchTree<T, Allocator>::template iterator<Tag>, bool> BinarySearchTree<T, Allocator>::emplace(Args&&... args) {
    std::pair<iterator<Tag>, bool> result;

    auto inserter = [&result, this](auto&& arg) {
      node_type* node = tree_->Find(arg, *tree_->Top());

      if (node != nullptr) {
        result = {iterator<Tag>(node), false};
      } else {
        node_type* new_node = allocator_.allocate(1);
        std::allocator_traits<Allocator>::construct(allocator_, new_node, arg);
        ++size_;
        tree_->Insert(new_node, *tree_->Top());
        result = {iterator<Tag>(new_node), true};
      } 
        
    };

    value_type buffer[] = { (inserter(std::forward<Args>(args)), 0)... };

    return result;

}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator<Tag> BinarySearchTree<T, Allocator>::upper_bound(const value_type& value) {
  node_type* current = *tree_->Top();
  node_type* lower_bound_node = nullptr;

  while (current != nullptr) {
      if (value < current->value_) {
          lower_bound_node = current;
          current = current->left;
      } else {
          current = current->right;
      }
  }

  if (lower_bound_node != nullptr) {
      return iterator_type<Tag>(lower_bound_node);
  } else {
      return end<Tag>();
  }
}

template <typename T, typename Allocator>
template <tag Tag>
typename BinarySearchTree<T, Allocator>::template iterator<Tag> BinarySearchTree<T, Allocator>::lower_bound(const value_type& value) {
    node_type* current = *tree_->Top();
    node_type* lower_bound_node = nullptr;

    while (current != nullptr) {
        if (value > current->value_) {
            lower_bound_node = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }

    if (lower_bound_node != nullptr) {
        return iterator_type<Tag>(lower_bound_node);
    } else {
        return end<Tag>();
    }
}
