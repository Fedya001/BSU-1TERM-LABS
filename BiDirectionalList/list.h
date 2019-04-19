//
// Created by user on 25.12.2018.
//

#ifndef BIDIRECTIONALLIST_LIST_H
#define BIDIRECTIONALLIST_LIST_H

//Напишите реализацию для класса BiDirectionalList и тесты к нему.

//Предусмотрите обработку ошибок (выход за границы массива, передача неверного
//итератора в метод и т. д.) с использованием механизма исключений.

//-------------------------------------------------------------------------------

#include <stdexcept>
#include <vector>
#include <iterator>
#include <functional>

template<typename T>
class BiDirectionalList {
 protected:
  struct Node;

 public:
  class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    T& operator*() const;
    T* operator->() const;

    Iterator& operator++();
    const Iterator operator++(int);

    Iterator& operator--();
    const Iterator operator--(int);

    Iterator& operator=(const Iterator& other);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;

   private:
    friend class BiDirectionalList;

    const BiDirectionalList* const list_;
    Node* node_;

    Iterator(const BiDirectionalList* const list, Node* node)
        : list_(list), node_(node) {}
  };

  class ConstIterator :
      public std::iterator<std::bidirectional_iterator_tag, T> {
   public:
    const T& operator*() const;
    const T* operator->() const;

    ConstIterator& operator++();
    const ConstIterator operator++(int);

    ConstIterator& operator--();
    const ConstIterator operator--(int);

    ConstIterator& operator=(const ConstIterator& other);
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;

   private:
    friend class BiDirectionalList;

    const BiDirectionalList* const list_;
    const Node* node_;

    ConstIterator(const BiDirectionalList* const list, Node* node)
        : list_(list), node_(node) {}
  };

  BiDirectionalList() : size_(0), first_(nullptr), last_(nullptr) {}

  template<typename Container>
  explicit BiDirectionalList(const Container&);

  ~BiDirectionalList() { Clear(); }

  bool IsEmpty() const;

  size_t Size() const;

  void Clear();

  Iterator begin();
  Iterator end();

  ConstIterator begin() const;
  ConstIterator end() const;

  std::vector<T> AsArray() const;

  void InsertBefore(Iterator position, const T& value);
  void InsertBefore(Iterator position, T&& value);

  void InsertAfter(Iterator position, const T& value);
  void InsertAfter(Iterator position, T&& value);

  void PushBack(const T& value);
  void PushBack(T&& value);

  void PushFront(const T& value);
  void PushFront(T&& value);

  void Erase(Iterator position);

  void PopFront();
  void PopBack();

  T Front() const;
  T Back() const;

  Iterator Find(const T& value);
  ConstIterator Find(const T& value) const;

  Iterator Find(std::function<bool(const T&)> predicate);
  ConstIterator Find(std::function<bool(const T&)> predicate) const;

 protected:
  struct Node {
    Node(const T& value);
    Node(T&& value);

    T value_;
    Node* next_node_;
    Node* previous_node_;
  };

  size_t size_;

  Node* first_;
  Node* last_;

  void InsertBefore(Node* existing_node, Node* new_node);
  void InsertAfter(Node* existing_node, Node* new_node);
  void Erase(Node* node);
};

// |--------------------------------------------------------------------------------------|
// |----------------------------- Iterator methods declaration ---------------------------|
// |--------------------------------------------------------------------------------------|
template<typename T>
T& BiDirectionalList<T>::Iterator::operator*() const {
  if (node_ == nullptr) {
    throw std::invalid_argument("operator* from end() iterator");
  }
  return node_->value_;
}

template<typename T>
T* BiDirectionalList<T>::Iterator::operator->() const {
  if (node_ == nullptr) {
    throw std::invalid_argument("operator* from end() iterator");
  }
  return &(node_->value_);
}

template<typename T>
typename BiDirectionalList<T>::Iterator& BiDirectionalList<T>::Iterator::operator++() {
  if (node_ == nullptr) {
    throw std::out_of_range("Trying to increment end() iterator");
  }
  node_ = node_->next_node_;
  return *this;
}

template<typename T>
const typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::Iterator::operator++(int) {
  Iterator temp = *this;
  ++(*this);
  return temp;
}

template<typename T>
typename BiDirectionalList<T>::Iterator& BiDirectionalList<T>::Iterator::operator--() {
  if (node_ == list_->first_) {
    throw std::out_of_range("Trying to decrement begin() iterator");
  }
  node_ = node_ ? node_->previous_node_ : list_->last_;
  return *this;
}

template<typename T>
const typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::Iterator::operator--(int) {
  Iterator temp = *this;
  --(*this);
  return temp;
}

template<typename T>
typename BiDirectionalList<T>::Iterator& BiDirectionalList<T>::Iterator::operator=(const BiDirectionalList::Iterator& other) {
  if (list_ != other.list_) {
    throw std::invalid_argument("Trying to assign iterator from another list");
  }
  node_ = other.node_;
  return *this;
}

template<typename T>
bool BiDirectionalList<T>::Iterator::operator==(const BiDirectionalList::Iterator& other) const {
  return node_ == other.node_;
}

template<typename T>
bool BiDirectionalList<T>::Iterator::operator!=(const BiDirectionalList::Iterator& other) const {
  return node_ != other.node_;
}
// |--------------------------------------------------------------------------------------|
// |----------------------------- Iterator methods declaration ---------------------------|
// |--------------------------------------------------------------------------------------|




// |------------------------------------------------------------------------------------------|
// |---------------------------- ConstIterator methods declaration ---------------------------|
// |------------------------------------------------------------------------------------------|
template<typename T>
const T& BiDirectionalList<T>::ConstIterator::operator*() const {
  if (node_ == nullptr) {
    throw std::invalid_argument("operator* from end() iterator");
  }
  return node_->value_;
}

template<typename T>
const T* BiDirectionalList<T>::ConstIterator::operator->() const {
  if (node_ == nullptr) {
    throw std::invalid_argument("operator* from end() iterator");
  }
  return &(node_->value_);
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator& BiDirectionalList<T>::ConstIterator::operator++() {
  if (node_ == nullptr) {
    throw std::out_of_range("Trying to increment end() iterator");
  }
  node_ = node_->next_node_;
  return *this;
}

template<typename T>
const typename BiDirectionalList<T>::ConstIterator BiDirectionalList<T>::ConstIterator::operator++(int) {
  ConstIterator temp = *this;
  ++(*this);
  return temp;
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator& BiDirectionalList<T>::ConstIterator::operator--() {
  if (node_ == list_->first_) {
    throw std::out_of_range("Trying to decrement begin() iterator");
  }
  node_ = node_ ? node_->previous_node_ : list_->last_;
  return *this;
}

template<typename T>
const typename BiDirectionalList<T>::ConstIterator BiDirectionalList<T>::ConstIterator::operator--(int) {
  ConstIterator temp = *this;
  --(*this);
  return temp;
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator& BiDirectionalList<T>::ConstIterator::operator=(const BiDirectionalList::ConstIterator& other) {
  if (list_ != other.list_) {
    throw std::invalid_argument("Trying to assign iterator from another list");
  }
  node_ = other.node_;
  return *this;
}

template<typename T>
bool BiDirectionalList<T>::ConstIterator::operator==(const BiDirectionalList::ConstIterator& other) const {
  return node_ == other.node_;
}

template<typename T>
bool BiDirectionalList<T>::ConstIterator::operator!=(const BiDirectionalList::ConstIterator& other) const {
  return node_ != other.node_;
}
// |------------------------------------------------------------------------------------------|
// |---------------------------- ConstIterator methods declaration ---------------------------|
// |------------------------------------------------------------------------------------------|





// |---------------------------------------------------------------------------------|
// |---------------------------- Node methods declaration ---------------------------|
// |---------------------------------------------------------------------------------|
template<typename T>
BiDirectionalList<T>::Node::Node(const T& value) : value_(value),
                                                   next_node_(nullptr),
                                                   previous_node_(nullptr) {}

template<typename T>
BiDirectionalList<T>::Node::Node(T&& value) : value_(value),
                                              next_node_(nullptr),
                                              previous_node_(nullptr) {
  value = T();
}

// |---------------------------------------------------------------------------------|
// |---------------------------- Node methods declaration ---------------------------|
// |---------------------------------------------------------------------------------|




// |---------------------------------------------------------------------------------------------------|
// |------------------------------- BiDirectionalList methods declaration -----------------------------|
// |---------------------------------------------------------------------------------------------------|
template<typename T>
template<typename Container>
BiDirectionalList<T>::BiDirectionalList(const Container& container) : size_(0), first_(nullptr), last_(nullptr) {
  for (const T& item : container) {
    PushBack(item);
  }
}

template<typename T>
bool BiDirectionalList<T>::IsEmpty() const {
  return size_ == 0;
}

template<typename T>
size_t BiDirectionalList<T>::Size() const {
  return size_;
}

template<typename T>
void BiDirectionalList<T>::Clear() {
  auto iterator = begin();
  while (iterator != end()) {
    auto next_iterator = ++iterator;
    iterator = next_iterator;
  }
  size_ = 0;
}

template<typename T>
typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::begin() {
  return {this, first_};
}

template<typename T>
typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::end() {
  return {this, nullptr};
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator BiDirectionalList<T>::begin() const {
  return {this, first_};
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator BiDirectionalList<T>::end() const {
  return {this, nullptr};
}

template<typename T>
std::vector<T> BiDirectionalList<T>::AsArray() const {
  std::vector<T> array;
  for (auto iterator = begin(); iterator != end(); ++iterator) {
    array.push_back(*iterator);
  }
  return array;
}

template<typename T>
void BiDirectionalList<T>::InsertBefore(BiDirectionalList::Iterator position, const T& value) {
  InsertBefore(position.node_, new Node(value));
}

template<typename T>
void BiDirectionalList<T>::InsertBefore(BiDirectionalList::Iterator position, T&& value) {
  InsertBefore(position.node_, std::move(new Node(value)));
}

template<typename T>
void BiDirectionalList<T>::InsertAfter(BiDirectionalList::Iterator position, const T& value) {
  InsertAfter(position.node_, new Node(value));
}

template<typename T>
void BiDirectionalList<T>::InsertAfter(BiDirectionalList::Iterator position, T&& value) {
  InsertAfter(position.node_, std::move(new Node(value)));
}

template<typename T>
void BiDirectionalList<T>::PushBack(const T& value) {
  InsertAfter(last_, new Node(value));
}

template<typename T>
void BiDirectionalList<T>::PushBack(T&& value) {
  InsertAfter(last_, new Node(std::move(value)));
}

template<typename T>
void BiDirectionalList<T>::PushFront(const T& value) {
  InsertBefore(first_, new Node(value));
}

template<typename T>
void BiDirectionalList<T>::PushFront(T&& value) {
  InsertBefore(first_, new Node(std::move(value)));
}

template<typename T>
void BiDirectionalList<T>::Erase(BiDirectionalList::Iterator position) {
  if (position.node_ == nullptr) {
    throw std::invalid_argument("trying to erase end()");
  }
  Erase(position.node_);
}

template<typename T>
void BiDirectionalList<T>::PopFront() {
  Erase(first_);
}

template<typename T>
void BiDirectionalList<T>::PopBack() {
  Erase(last_);
}

template<typename T>
T BiDirectionalList<T>::Front() const {
  if (IsEmpty()) {
    throw std::out_of_range("Trying to access front element in empty list");
  }
  return *begin();
}

template<typename T>
T BiDirectionalList<T>::Back() const {
  if (IsEmpty()) {
    throw std::out_of_range("Trying to access back element in empty list");
  }
  return *(--end());
}

template<typename T>
typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::Find(const T& value) {
  for (auto iterator = begin(); iterator != end(); ++iterator) {
    if (*iterator == value) {
      return iterator;
    }
  }
  return end();
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator BiDirectionalList<T>::Find(const T& value) const {
  for (auto iterator = begin(); iterator != end(); ++iterator) {
    if (*iterator == value) {
      return iterator;
    }
  }
  return end();
}

template<typename T>
typename BiDirectionalList<T>::Iterator BiDirectionalList<T>::Find(std::function<bool(const T&)> predicate) {
  for (auto iterator = begin(); iterator != end(); ++iterator) {
    if (predicate(*iterator)) {
      return iterator;
    }
  }
  return end();
}

template<typename T>
typename BiDirectionalList<T>::ConstIterator BiDirectionalList<T>::Find(std::function<bool(const T&)> predicate) const {
  for (auto iterator = begin(); iterator != end(); ++iterator) {
    if (predicate(*iterator)) {
      return iterator;
    }
  }
  return end();
}

template<typename T>
void BiDirectionalList<T>::InsertBefore(BiDirectionalList::Node* existing_node, BiDirectionalList::Node* new_node) {
  if (size_ == 0) {
    first_ = last_ = new_node;
    ++size_;
    return;
  }
  if (existing_node != nullptr) {
    if (existing_node != first_) {
      Node* previous = existing_node->previous_node_;
      previous->next_node_ = new_node;
      new_node->previous_node_ = previous;
    } else {
      first_ = new_node;
    }
    new_node->next_node_ = existing_node;
    existing_node->previous_node_ = new_node;
  } else {
    last_->next_node_ = new_node;
    new_node->previous_node_ = last_;
    last_ = new_node;
  }
  ++size_;
}

template<typename T>
void BiDirectionalList<T>::InsertAfter(BiDirectionalList::Node* existing_node, BiDirectionalList::Node* new_node) {
  if (existing_node != nullptr) {
    InsertBefore(existing_node->next_node_, new_node);
  } else {
    InsertBefore(nullptr, new_node);
  }
}

template<typename T>
void BiDirectionalList<T>::Erase(BiDirectionalList::Node* node) {
  if (size_ == 1) {
    delete first_;
    first_ = last_ = nullptr;
  } else if (node == first_) {
    first_ = first_->next_node_;
    delete first_->previous_node_;
    first_->previous_node_ = nullptr;
  } else if (node == last_) {
    last_ = last_->previous_node_;
    delete last_->next_node_;
    last_->next_node_ = nullptr;
  } else {
    node->previous_node_->next_node_ = node->next_node_;
    node->next_node_->previous_node_ = node->previous_node_;
    delete node;
  }
  --size_;
}
// |---------------------------------------------------------------------------------------------------|
// |------------------------------- BiDirectionalList methods declaration -----------------------------|
// |---------------------------------------------------------------------------------------------------|

#endif //BIDIRECTIONALLIST_LIST_H
