#pragma once

template<typename T>
struct Node {
    Node* parent;
    Node* left;
    Node* right;
    const T value_;
    Node() = default;
    Node(T value) : value_(value), left(nullptr), right(nullptr), parent(nullptr) {} 
};

template<typename T, typename Allocator  = std::allocator<Node<T>>>
class Tree {
public:
    typedef T value_type;
    typedef Node<T> node_type;
    Tree() : top_(nullptr) {}

    virtual ~Tree() = default;

    virtual node_type* Remove(node_type*& current, value_type value) = 0;
    virtual node_type* Extract(value_type value) = 0;
    virtual Tree& Insert(node_type*& node, node_type*& prev) = 0;
    virtual node_type* Find(value_type value, node_type*& node) = 0;
    virtual node_type* DeepCopyTree(node_type* node) = 0;
    virtual void DestroyTree(node_type* node) = 0;
    virtual bool EqualTrees(node_type* node1, node_type* node2) = 0;

    node_type* const* cTop() const { return &top_; }
    node_type** Top() { return &top_; }
    Tree& SetTop(node_type* node) { top_ = node; return *this; }
    
protected:
  node_type* top_;
  Allocator allocator_;
};


template<typename T, typename Allocator = std::allocator<Node<T>>>
class BST : public Tree<T, Allocator> {
    
public:
    using typename Tree<T, Allocator>::value_type;
    using typename Tree<T, Allocator>::node_type;
    using Tree<T, Allocator>::top_;
    using Tree<T, Allocator>::Tree;
    using Tree<T, Allocator>::allocator_;

    ~BST() override { DestroyTree(top_); }
    node_type* Remove(node_type*& current, value_type value) override;
    node_type* Extract(value_type value) override;
    Tree<T, Allocator>& Insert(node_type*& node, node_type*& prev) override;
    node_type* Find(value_type value, node_type*& node) override;
    node_type* DeepCopyTree(node_type* node) override;
    void DestroyTree(node_type* node) override;
    bool EqualTrees(node_type* node1, node_type* node2) override;
};

template<typename T, typename Allocator>
Tree<T, Allocator>& BST<T, Allocator>::Insert(node_type*& node, node_type*& current) {
  if (current == nullptr) {
    current = node;
    if (current == top_) {
      node->parent = nullptr;
    }
  } else {
    if (node->value_ < current->value_) {
      if (current->left == nullptr) {
        current->left = node; 
        node->parent = current;
      } else {
        Insert(node, current->left);
      }
    } else if (node->value_ > current->value_) {
      if (current->right == nullptr) {
        current->right = node; 
        node->parent = current;
      } else {
        Insert(node, current->right); 
      }
    }
  }
  return *this;
}

template <typename T, typename Allocator>
typename BST<T, Allocator>::node_type* BST<T, Allocator>::Find(value_type value, node_type*& node) {
  if (node == nullptr) { return nullptr; }
  if (value == node->value_) { return node; }
  if (node->value_ < value && node->right != nullptr) { return Find(value, node->right); }
  if (node->value_ > value && node->left != nullptr) { return Find(value, node->left); } 
  return nullptr;
}

template<typename T, typename Allocator>
typename BST<T, Allocator>::node_type* BST<T, Allocator>::Remove(node_type*& current, value_type value) {
    if (current == nullptr) {
        return nullptr;
    }

    if (value < current->value_) {
        current->left = Remove(current->left, value);
        if (current->left != nullptr) {
            current->left->parent = current; 
        }
    } else if (value > current->value_) {
        current->right = Remove(current->right, value);
        if (current->right != nullptr) {
            current->right->parent = current; 
        }
    } else {
        if (current->left == nullptr) {
            Node<T>* temp = current->right;
            if (temp != nullptr) {
                temp->parent = current->parent; 
            }
            delete current;
            return temp;
        } else if (current->right == nullptr) {
            Node<T>* temp = current->left;
            if (temp != nullptr) {
                temp->parent = current->parent;
            }
            delete current;
            return temp;
        }

        Node<T>* succ = current->right;
        while (succ->left != nullptr) {
            succ = succ->left;
        }

        const_cast<T&>(current->value_) = succ->value_;
        current->right = Remove(current->right, succ->value_);
        if (current->right != nullptr) {
            current->right->parent = current;
        }
    }

    return current;
}



template <typename T, typename Allocator>
typename BST<T, Allocator>::node_type* BST<T, Allocator>::Extract(value_type value) {
  node_type temp = *Find(value, top_);
  node_type* ex = &temp;
  if (ex == nullptr) {
    return nullptr;
  }

  top_ = Remove(top_, value);

  ex->parent = nullptr; 
  ex->left = nullptr;
  ex->right = nullptr;
  return ex;
}

template<typename T, typename Allocator>
void BST<T, Allocator>::DestroyTree(node_type* node) {
    if (node != nullptr) {
        DestroyTree(node->left);
        DestroyTree(node->right);
        std::allocator_traits<Allocator>::destroy(allocator_, node);
        allocator_.deallocate(node, 1);
    }
    top_ = nullptr;
}

template<typename T, typename Allocator>
typename BST<T, Allocator>::node_type* BST<T, Allocator>::DeepCopyTree(node_type* node) {
  if (node == nullptr) {
    return nullptr;
  }

  Node<T>* new_node = allocator_.allocate(1);
  std::allocator_traits<Allocator>::construct(allocator_, new_node, node->value_);

  new_node->left = DeepCopyTree(node->left);
  if (new_node->left != nullptr) {
    new_node->left->parent = new_node;
  }

  new_node->right = DeepCopyTree(node->right);
  if (new_node->right != nullptr) {
    new_node->right->parent = new_node;
  }

  return new_node;
}

template<typename T, typename Allocator>
bool BST<T, Allocator>::EqualTrees(node_type* node1, node_type* node2) {
  if (node1 == nullptr && node2 == nullptr) {
      return true;
  }
  if (node1 == nullptr || node2 == nullptr) {
      return false;
  }
  
  return (node1->value_ == node2->value_) && EqualTrees(node1->left, node2->left) && EqualTrees(node1->right, node2->right);
}