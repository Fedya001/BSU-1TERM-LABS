#include <algorithm>
#include <iostream>
#include <cassert>
#include <random>
#include <string>
#include <vector>
#include <set>

class BinarySearchTree {
 public:
  BinarySearchTree();
  ~BinarySearchTree();
  bool Contains(int value) const;
  void Add(int value);
  void Erase(int value);
  int Size() const;
  bool IsEmpty() const;
  std::vector<int> ToSortedArray() const;

 protected:
  struct Node {
    int value;

    Node *parent;
    Node *left;
    Node *right;

    Node(int value, Node *parent) : value(value), parent(parent),
                                    left(nullptr), right(nullptr) {}

    ~Node() {
      delete left;
      delete right;
    }
  };

  // Указатель на корневую вершину дерева и общее количество вершин соответственно
  Node *root_;
  int size_;

  // Вспомогательный метод, возвращающий указатель на одну из вершин со
  // значением 'value' (любую из них), либо nullptr при отсутствии таких вершин.
  Node *Find(int value) const;

  // Вспомогательный метод, позволяющий "отвязать" вершину от дерева поиска.
  // После выполнения данной операции, вершина 'target_node' перестаёт входить
  // в поддерево, образуемое вершиной 'root_', но удаления данной вершины не
  // происходит.
  void Detach(Node *target_node);

  // Вспомагательный метод, который вставляет узел target_node
  // в дерево вместо узла node.
  // Последний полностью вырезается из дерева
  void ReplaceNode(Node *node, Node *target_node);

  // Вспомогательные методы, которые позволяют получать указатели на самую
  // левую и самую правую вершины в поддереве, образуемом вершиной node,
  // соответствтенно.
  Node *GetLeftmostNode(Node *node) const;
  Node *GetRightmostNode(Node *node) const;

  // Вспомогательные метод, дописывающий все элементы в поддереве, образуемом
  // вершиной node, в конец вектора *res в порядке неубывания хранимых значений.
  void AppendToSortedArray(Node *node, std::vector<int> *res) const;
};

BinarySearchTree::BinarySearchTree() : root_(nullptr), size_(0) {}

BinarySearchTree::~BinarySearchTree() {
  delete root_;
}

bool BinarySearchTree::Contains(int value) const {
  return Find(value) != nullptr;
}

void BinarySearchTree::Add(int value) {
  Node *temp = new Node{value, nullptr};

  if (!IsEmpty()) {
    Node *prev = nullptr, *node = root_;
    while (node != nullptr) {
      prev = node;
      node = (value < node->value) ?
             node->left : node->right;
    }

    temp->parent = prev;
    if (value < prev->value) {
      prev->left = temp;
    } else {
      prev->right = temp;
    }
  } else {
    root_ = temp;
  }
  ++size_;
}

void BinarySearchTree::Erase(int value) {
  Node *node = Find(value);
  if (node == nullptr) return;

  Node *removing_node = (node->left == nullptr || node->right == nullptr) ?
                        node : GetLeftmostNode(node->right);

  Node *any_kid = (removing_node->left) ?
                  removing_node->left : removing_node->right;

  Node *parent = removing_node->parent;
  if (any_kid != nullptr) {
    any_kid->parent = parent;
  }

  if (parent == nullptr) {
    root_ = any_kid;
  } else {
    if (parent->left == removing_node) {
      parent->left = any_kid;
    } else {
      parent->right = any_kid;
    }
  }

  if (node != removing_node) {
    ReplaceNode(node, removing_node);
    if (root_ == node) {
      root_ = removing_node;
    }
  }
  Detach(node);
  delete node;
  --size_;
}

int BinarySearchTree::Size() const {
  return size_;
}

bool BinarySearchTree::IsEmpty() const {
  return Size() == 0;
}

std::vector<int> BinarySearchTree::ToSortedArray() const {
  std::vector<int> buffer;
  if (IsEmpty()) return buffer;

  AppendToSortedArray(root_, &buffer);
  return buffer;
}

BinarySearchTree::Node *BinarySearchTree::Find(int value) const {
  Node *node = root_;
  while (node != nullptr && node->value != value) {
    node = (value < node->value) ?
           node->left : node->right;
  }
  return node;
}

void BinarySearchTree::Detach(Node *target_node) {
  target_node->left = target_node->right
      = target_node->parent = nullptr;
}

void BinarySearchTree::ReplaceNode(Node *node, Node *target_node) {
  target_node->left = node->left;
  if (node->left != nullptr) {
    node->left->parent = target_node;
  }

  target_node->right = node->right;
  if (node->right != nullptr) {
    node->right->parent = target_node;
  }

  target_node->parent = node->parent;
  if (node->parent != nullptr) {
    if (node == node->parent->left) {
      node->parent->left = target_node;
    } else {
      node->parent->right = target_node;
    }
  }
}

BinarySearchTree::Node *BinarySearchTree::GetLeftmostNode
    (BinarySearchTree::Node *node) const {
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

BinarySearchTree::Node *BinarySearchTree::GetRightmostNode
    (BinarySearchTree::Node *node) const {
  while (node->right != nullptr) {
    node = node->right;
  }
  return node;
}

void BinarySearchTree::AppendToSortedArray
    (BinarySearchTree::Node *node, std::vector<int> *res) const {
  if (node->left != nullptr) {
    AppendToSortedArray(node->left, res);
  }
  res->push_back(node->value);
  if (node->right != nullptr) {
    AppendToSortedArray(node->right, res);
  }
}

int main() {
  const int kQueriesCount = 50'000;
  const std::vector<int>
      kElementsMaxValues({1, 2, 5, 13, 42, 1024, 1'000'000'000});

  std::mt19937_64 random_generator(2018);
  for (const int max_element_value : kElementsMaxValues) {
    std::vector<int> elements;
    BinarySearchTree tree;

    for (int i = 0; i < kQueriesCount; ++i) {
      int query_type = random_generator() % 3;

      switch (query_type) {
        case 0: {
          int value;
          if (elements.empty() || random_generator() % 100 < 75) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          elements.push_back(value);
          sort(elements.begin(), elements.end());  // Never do like this :)
          tree.Add(value);
          break;
        }
        case 1: {
          int value;
          if (elements.empty() || random_generator() % 100 < 40) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          bool result = tree.Contains(value);
          bool expected_result =
              std::find(elements.begin(), elements.end(), value)
                  != elements.end();
          assert(result == expected_result);
          break;
        }
        case 2: {
          int value;
          if (elements.empty() || random_generator() % 100 < 25) {
            value = random_generator() % max_element_value;
            value -= max_element_value / 2;
          } else {
            value = elements[random_generator() % elements.size()];
          }

          auto element_it = std::find(elements.begin(), elements.end(), value);
          if (element_it != elements.end()) {
            elements.erase(element_it);
          }
          tree.Erase(value);
          break;
        }
        default: {
          break;  // IMPOSSIBLE
        }
      }

      assert(elements.size() == tree.Size());
      assert(elements == tree.ToSortedArray());
    }

    std::cout << "Passed: max_element_value = " << max_element_value << std::endl;
  }

  return 0;
}
