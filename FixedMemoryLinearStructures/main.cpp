#include <iostream>
#include <cassert>
#include <string>
#include <vector>

class Deque {
 public:
  explicit Deque(int max_size) : data_(max_size),
                                 head_(0), tail_(0), size_(0) {}

  int Size() const {
    return size_;
  }

  int MaxSize() const {
    return data_.size();
  }

  bool IsEmpty() const {
    return (Size() == 0);
  }

  bool IsFull() const {
    return (Size() == MaxSize());
  }

  bool PushBack(int element) {
    if (IsFull())
      return false;

    data_[tail_] = element;
    tail_ = GetNextIndex(tail_);
    ++size_;
    return true;
  }

  int Back(int default_value = 0) const {
    if (IsEmpty())
      return default_value;

    return data_[GetPreviousIndex(tail_)];
  }

  bool PopBack() {
    if (IsEmpty())
      return false;

    tail_ = GetPreviousIndex(tail_);
    --size_;
    return true;
  }

  bool PushFront(int element) {
    if (IsFull())
      return false;

    head_ = GetPreviousIndex(head_);
    data_[head_] = element;
    ++size_;
    return true;
  }

  int Front(int default_value = 0) const {
    if (IsEmpty())
      return default_value;

    return data_[head_];
  }

  bool PopFront() {
    if (IsEmpty())
      return false;

    head_ = GetNextIndex(head_);
    --size_;
    return true;
  }

 private:
  int GetNextIndex(int index) const {
    return (index + 1) % MaxSize();
  }
  int GetPreviousIndex(int index) const {
    return (index - 1 + MaxSize()) % MaxSize();
  }

  std::vector<int> data_;
  int head_;
  int tail_;
  int size_;
};

class Queue {
 public:
  explicit Queue(int max_size) : data_(max_size) {}

  bool Push(int value) {
    return data_.PushBack(value);
  }

  bool Pop() {
    return data_.PopFront();
  }

  int Front(int default_value = 0) const {
    return data_.Front(default_value);
  }

  int Size() const {
    return data_.Size();
  }

  int MaxSize() const {
    return data_.MaxSize();
  }

  bool IsEmpty() const {
    return data_.IsEmpty();
  }

  bool IsFull() const {
    return data_.IsFull();
  }

 private:
  Deque data_;
};

class Stack {
 public:
  explicit Stack(int max_size) : data_(max_size) {}

  bool Push(int value) {
    return data_.PushBack(value);
  }

  bool Pop() {
    return data_.PopBack();
  }

  int Top(int default_value = 0) const {
    return data_.Back(default_value);
  }

  int Size() const {
    return data_.Size();
  }

  int MaxSize() const {
    return data_.MaxSize();
  }

  bool IsEmpty() const {
    return data_.IsEmpty();
  }

  bool IsFull() const {
    return data_.IsFull();
  }

 private:
  Deque data_;
};

int main() {
  {
    const Deque deque(5);
    assert(deque.IsEmpty());
    assert(!deque.IsFull());
    assert(deque.Size() == 0);
    assert(deque.MaxSize() == 5);
    assert(deque.Front() == 0);
    assert(deque.Front(0) == 0);
    assert(deque.Back() == 0);
    assert(deque.Back(0) == 0);
    std::cout << "PASSED: Deque > Const" << std::endl;
  }
  {
    Deque deque(5);

    assert(!deque.PopBack());

    assert(deque.Size() == 0);
    assert(deque.Back() == 0);
    assert(deque.Back(42) == 42);
    assert(deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushBack(13));
    assert(deque.PushBack(123));
    assert(deque.PushBack(123456));

    assert(deque.Size() == 3);
    assert(deque.Back() == 123456);
    assert(deque.Back(42) == 123456);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PopBack());

    assert(deque.Size() == 2);
    assert(deque.Back() == 123);
    assert(deque.Back(42) == 123);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushBack(3));
    assert(deque.PushBack(4));
    assert(deque.PushBack(5));

    assert(deque.Size() == 5);
    assert(deque.Back() == 5);
    assert(deque.Back(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    assert(!deque.PushBack(99));

    assert(deque.Size() == 5);
    assert(deque.Back() == 5);
    assert(deque.Back(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    std::cout << "PASSED: Deque > Back" << std::endl;
  }
  {
    Deque deque(5);

    assert(!deque.PopFront());

    assert(deque.Size() == 0);
    assert(deque.Front() == 0);
    assert(deque.Front(42) == 42);
    assert(deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushFront(13));
    assert(deque.PushFront(123));
    assert(deque.PushFront(123456));

    assert(deque.Size() == 3);
    assert(deque.Front() == 123456);
    assert(deque.Front(42) == 123456);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PopFront());

    assert(deque.Size() == 2);
    assert(deque.Front() == 123);
    assert(deque.Front(42) == 123);
    assert(!deque.IsEmpty());
    assert(!deque.IsFull());

    assert(deque.PushFront(3));
    assert(deque.PushFront(4));
    assert(deque.PushFront(5));

    assert(deque.Size() == 5);
    assert(deque.Front() == 5);
    assert(deque.Front(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    assert(!deque.PushFront(99));

    assert(deque.Size() == 5);
    assert(deque.Front() == 5);
    assert(deque.Front(42) == 5);
    assert(!deque.IsEmpty());
    assert(deque.IsFull());

    std::cout << "PASSED: Deque > Front" << std::endl;
  }
  {
    Deque deque(3);

    assert(deque.PushFront(1));

    assert(deque.Size() == 1);
    assert(deque.Back() == 1);
    assert(deque.Front() == 1);

    assert(deque.PushBack(2));

    assert(deque.Size() == 2);
    assert(deque.Back() == 2);
    assert(deque.Front() == 1);

    assert(deque.PushFront(0));

    assert(deque.Size() == 3);
    assert(deque.Back() == 2);
    assert(deque.Front() == 0);

    assert(deque.PopFront());

    assert(deque.Size() == 2);
    assert(deque.Back() == 2);
    assert(deque.Front() == 1);

    assert(deque.PushBack(3));

    assert(deque.Size() == 3);
    assert(deque.Back() == 3);
    assert(deque.Front() == 1);

    assert(deque.PopFront());

    assert(deque.Size() == 2);
    assert(deque.Back() == 3);
    assert(deque.Front() == 2);

    assert(deque.PushBack(4));

    assert(deque.Size() == 3);
    assert(deque.Back() == 4);
    assert(deque.Front() == 2);

    assert(deque.PopBack());

    assert(deque.Size() == 2);
    assert(deque.Back() == 3);
    assert(deque.Front() == 2);

    std::cout << "PASSED: Deque > Mixed" << std::endl;
  }
  {
    const Queue queue(2);
    assert(queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Size() == 0);
    assert(queue.MaxSize() == 2);
    assert(queue.Front() == 0);
    assert(queue.Front(0) == 0);
    std::cout << "PASSED: Queue > Const" << std::endl;
  }
  {
    Queue queue(2);

    assert(queue.Size() == 0);
    assert(queue.MaxSize() == 2);
    assert(queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 0);
    assert(queue.Front(42) == 42);

    assert(!queue.Pop());

    assert(queue.Size() == 0);
    assert(queue.MaxSize() == 2);
    assert(queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 0);
    assert(queue.Front(42) == 42);

    assert(queue.Push(1));

    assert(queue.Size() == 1);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 1);
    assert(queue.Front(42) == 1);

    assert(queue.Push(2));

    assert(queue.Size() == 2);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(queue.IsFull());
    assert(queue.Front() == 1);
    assert(queue.Front(42) == 1);

    assert(!queue.Push(3));

    assert(queue.Size() == 2);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(queue.IsFull());
    assert(queue.Front() == 1);
    assert(queue.Front(42) == 1);

    assert(queue.Pop());

    assert(queue.Size() == 1);
    assert(queue.MaxSize() == 2);
    assert(!queue.IsEmpty());
    assert(!queue.IsFull());
    assert(queue.Front() == 2);
    assert(queue.Front(42) == 2);

    std::cout << "PASSED: Queue > Main" << std::endl;
  }
  {
    const Stack stack(2);
    assert(stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Size() == 0);
    assert(stack.MaxSize() == 2);
    assert(stack.Top() == 0);
    assert(stack.Top(0) == 0);
    std::cout << "PASSED: Stack > Const" << std::endl;
  }
  {
    Stack stack(2);

    assert(stack.Size() == 0);
    assert(stack.MaxSize() == 2);
    assert(stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 0);
    assert(stack.Top(42) == 42);

    assert(!stack.Pop());

    assert(stack.Size() == 0);
    assert(stack.MaxSize() == 2);
    assert(stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 0);
    assert(stack.Top(42) == 42);

    assert(stack.Push(1));

    assert(stack.Size() == 1);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 1);
    assert(stack.Top(42) == 1);

    assert(stack.Push(2));

    assert(stack.Size() == 2);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(stack.IsFull());
    assert(stack.Top() == 2);
    assert(stack.Top(42) == 2);

    assert(!stack.Push(3));

    assert(stack.Size() == 2);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(stack.IsFull());
    assert(stack.Top() == 2);
    assert(stack.Top(42) == 2);

    assert(stack.Pop());

    assert(stack.Size() == 1);
    assert(stack.MaxSize() == 2);
    assert(!stack.IsEmpty());
    assert(!stack.IsFull());
    assert(stack.Top() == 1);
    assert(stack.Top(42) == 1);

    std::cout << "PASSED: Stack > Main" << std::endl;
  }

  return 0;
}
