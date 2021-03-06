#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <deque>

template<typename T>
class VectorInternalsAccessor;

template<class T>
class Vector {
 public:
  Vector() : size_(0), allocated_size_(1), data_(new T[1]) {}

  Vector(const Vector<T>& vector) : size_(vector.size_),
                                    allocated_size_(vector.allocated_size_),
                                    data_(new T[allocated_size_]) {
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = vector.data_[i];
    }
  }

  Vector& operator=(const Vector<T>& vector) {
    size_ = vector.size_;
    if (allocated_size_ != vector.allocated_size_) {
      delete[] data_;
      allocated_size_ = vector.allocated_size_;
      data_ = new T[allocated_size_];
    }
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = vector.data_[i];
    }
    return *this;
  }

  Vector(Vector<T>&& vector) : size_(vector.size_),
                               allocated_size_(vector.allocated_size_),
                               data_(vector.data_) {
    vector.size_ = 0;
    vector.allocated_size_ = 1;
    vector.data_ = new T[1];
  }

  Vector& operator=(Vector<T>&& vector) {
    size_ = vector.size_;
    delete[] data_;
    allocated_size_ = vector.allocated_size_;
    data_ = vector.data_;

    vector.size_ = 0;
    vector.allocated_size_ = 1;
    vector.data_ = new T[1];

    return *this;
  }

  ~Vector() {
    delete[] data_;
  }

  size_t Size() const {
    return size_;
  }

  bool IsEmpty() const {
    return size_ == 0;
  }

  bool IsFull() const {
    return size_ == allocated_size_;
  }

  void PushBack(const T& value) {
    if (IsFull()) {
      Relocate(allocated_size_ * 2);
    }
    data_[size_] = value;
    ++size_;
  }

  void PopBack() {
    assert(!IsEmpty());
    --size_;
    if (size_ * 4 < allocated_size_) {
      Relocate(allocated_size_ / 2);
    }
  }

  T& operator[](size_t index) {
    assert(index < size_);
    return data_[index];
  }

  const T& operator[](size_t index) const {
    assert(index < size_);
    return data_[index];
  }

  void PushFront(const T& value) {
    if (IsFull()) {
      Relocate(allocated_size_ * 2);
    }
    for (size_t i = size_; i >= 1; --i) {
      data_[i] = data_[i - 1];
    }
    data_[0] = value;
    ++size_;
  }

  void PopFront() {
    assert(!IsEmpty());
    for (size_t i = 0; i < size_ - 1; ++i) {
      data_[i] = data_[i + 1];
    }
    --size_;
    if (size_ * 4 < allocated_size_) {
      Relocate(allocated_size_ / 2);
    }
  }

  template<class... Args>
  void EmplaceBack(Args&& ... args) {
    if (IsFull()) {
      Relocate(allocated_size_ * 2);
    }
    data_[size_] = T(std::forward<Args>(args)...);
    ++size_;
  }

  template<class... Args>
  void EmplaceFront(Args&& ... args) {
    if (IsFull()) {
      Relocate(allocated_size_ * 2);
    }
    for (size_t i = size_; i >= 1; --i) {
      data_[i] = data_[i - 1];
    }
    data_[0] = T(std::forward<Args>(args)...);
    ++size_;
  }

  int Find(const T& value) const {
    for (size_t i = 0; i < size_; ++i) {
      if (data_[i] == value) {
        return i;
      }
    }
    return -1;
  }

 protected:
  friend class VectorInternalsAccessor<T>;  // DO_NOT_CHANGE

  size_t size_;
  size_t allocated_size_;

  T* data_;

  void Relocate(size_t new_size) {
    T* data_copy = data_;
    data_ = new T[new_size];
    for (size_t i = 0; i < size_; i++) {
      data_[i] = data_copy[i];
    }
    allocated_size_ = new_size;
    delete[] data_copy;
  }
};

// ==================== DO NOT EDIT THIS CLASS ==================
template<typename T>
class VectorInternalsAccessor {
 public:
  VectorInternalsAccessor() = delete;
  static size_t Size(const Vector<T>& vector) {
    return vector.size_;
  }
  static T* AllocData(const Vector<T>& vector) {
    return vector.data_;
  }
  static size_t AllocSize(const Vector<T>& vector) {
    return vector.allocated_size_;
  }
};

// ===============================================================
// Для тестирования группы закомментируйте или удалите строчку
// "#define SKIP_XXXXX" для соответствующей группы тестов.

// Подзадачи:
//    (1) Базовая функциональность и операции с хвостом вектора
// #define SKIP_BASIC
//    (2) Релоцирование памяти
// #define SKIP_RELOC
//    (3) : Работа с началом вектора
// #define SKIP_XFRONT
//    (4) : Find
// #define SKIP_FIND
//    (5) : Emplace
// #define SKIP_EMPLACE
//    (6) Копирование
// #define SKIP_COPY
//    (7) : Перемещение
// #define SKIP_MOVE
// ===============================================================

template<typename T>
std::string TypeName() {
  using T_no_ref = typename std::remove_reference<T>::type;
  std::string r;
  if (std::is_const<T_no_ref>::value) {
    r += "const ";
  }
  if (std::is_volatile<T_no_ref>::value) {
    r += "volatile ";
  }
  r += typeid(T_no_ref).name();
  if (std::is_lvalue_reference<T>::value) {
    r += "&";
  }
  if (std::is_rvalue_reference<T>::value) {
    r += "&&";
  }
  return r;
}

int main() {
#ifndef SKIP_BASIC
  {
    Vector<int> v;
    const Vector<int>& const_v = v;

    assert(const_v.Size() == 0 && const_v.IsEmpty());

    v.PushBack(42);
    assert(const_v.Size() == 1 && !const_v.IsEmpty());
    assert(v[0] == 42);
    assert(const_v[0] == 42);

    v[0] *= 10101;

    // Строчка ниже должна вызывать ошибку компиляции.
    // const_v[0] *= 10101;

    assert(v[0] == 424242);
    assert(const_v[0] == 424242);

    v.PopBack();
    assert(const_v.Size() == 0 && const_v.IsEmpty());

    assert(TypeName<decltype(v[0])>() == "i&");
    assert(TypeName<decltype(const_v[0])>() == "const i&");
  }
  std::cout << "[PASS] Basic" << std::endl;
#else
  std::cout << "[SKIPPED] Basic" << std::endl;
#endif  // SKIP_BASIC

#ifndef SKIP_RELOC
  {
    Vector<int> v;
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 1);
    for (int i = 0; i < 1025; ++i) {
      v.PushBack(i);
    }
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 2048);
    for (int i = 0; i < 1025; ++i) {
      assert(v[i] == i);
    }
  }
  std::cout << "[PASS] Relocation" << std::endl;
#else
  std::cout << "[SKIPPED] Relocation" << std::endl;
#endif  // SKIP_RELOC

#ifndef SKIP_XFRONT
  {
    Vector<int> v;

    v.PushBack(42);
    assert(v.Size() == 1 && !v.IsEmpty());
    assert(v[0] == 42);

    v.PushFront(1);
    assert(v.Size() == 2 && !v.IsEmpty());
    assert(v[0] == 1);
    assert(v[1] == 42);

    v.PopFront();
    assert(v.Size() == 1 && !v.IsEmpty());
    assert(v[0] == 42);

    v.PopBack();
    assert(v.Size() == 0 && v.IsEmpty());

    v.PushFront(777);
    assert(v.Size() == 1 && !v.IsEmpty());
    assert(v[0] == 777);

    v.PopFront();
    assert(v.Size() == 0 && v.IsEmpty());
  }
  std::cout << "[PASS] XFront" << std::endl;
#else
  std::cout << "[SKIPPED] XFront" << std::endl;
#endif  // SKIP_XFRONT

#ifndef SKIP_FIND
  {
    Vector<int> v;
    v.PushBack(0);
    v.PushBack(1);
    v.PushBack(2);
    v.PushBack(1);
    const auto& const_v = v;

    assert(const_v.Find(0) == 0);
    assert(const_v.Find(1) == 1);
    assert(const_v.Find(2) == 2);
    assert(const_v.Find(3) == -1);
  }
  std::cout << "[PASS] Find" << std::endl;
#else
  std::cout << "[SKIPPED] Find" << std::endl;
#endif  // SKIP_FIND

#ifndef SKIP_EMPLACE
  {
    struct MyStruct {
      int i_field;
      std::string s_field;
      MyStruct() : i_field(0), s_field("<EMPTY>") {}
      MyStruct(int i_field, std::string s_field)
          : i_field(i_field), s_field(std::move(s_field)) {}
    };
    Vector<MyStruct> v;

    v.EmplaceBack(MyStruct());
    v.EmplaceBack();
    v.EmplaceBack(42, "4242");
    v.EmplaceFront(MyStruct());
    v.EmplaceFront();
    v.EmplaceFront(1111, "2222");

    assert(v[0].i_field == 1111 && v[0].s_field == "2222");
    assert(v[1].i_field == 0 && v[1].s_field == "<EMPTY>");
    assert(v[2].i_field == 0 && v[2].s_field == "<EMPTY>");
    assert(v[3].i_field == 0 && v[3].s_field == "<EMPTY>");
    assert(v[4].i_field == 0 && v[4].s_field == "<EMPTY>");
    assert(v[5].i_field == 42 && v[5].s_field == "4242");
  }
  std::cout << "[PASS] Empalce" << std::endl;
#else
  std::cout << "[SKIPPED] Empalce" << std::endl;
#endif  // SKIP_EMPLACE

#ifndef SKIP_COPY
  {
    Vector<int> v;
    for (int i = 0; i < 67; ++i) {
      v.PushBack(i);
    }

    Vector<int> other(v);
    for (int i = 0; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }

    v[0] = 1111;
    other[1] = 2222;

    assert(v[0] == 1111);
    assert(v[1] == 1);
    assert(other[0] == 0);
    assert(other[1] == 2222);

    for (int i = 2; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }
  }
  {
    Vector<int> v;
    for (int i = 0; i < 67; ++i) {
      v.PushBack(i);
    }

    Vector<int> other = v;
    for (int i = 0; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }

    v[0] = 1111;
    other[1] = 2222;

    assert(v[0] == 1111);
    assert(v[1] == 1);
    assert(other[0] == 0);
    assert(other[1] == 2222);

    for (int i = 2; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }
  }
  {
    Vector<int> v;
    for (int i = 0; i < 67; ++i) {
      v.PushBack(i);
    }

    Vector<int> other;
    for (int i = 0; i < 6; ++i) {
      v.PushBack(i * 55);
    }

    other = v;
    for (int i = 0; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }

    v[0] = 1111;
    other[1] = 2222;

    assert(v[0] == 1111);
    assert(v[1] == 1);
    assert(other[0] == 0);
    assert(other[1] == 2222);

    for (int i = 2; i < 67; ++i) {
      assert(v[i] == i);
      assert(other[i] == i);
    }

    auto ret_type = TypeName<decltype(other = v)>();
    assert(ret_type == std::string(typeid(Vector<int>).name()) + "&");
  }
  std::cout << "[PASS] Copy" << std::endl;
#else
  std::cout << "[SKIPPED] Copy" << std::endl;
#endif  // SKIP_COPY

#ifndef SKIP_MOVE
  {
    Vector<int> v;
    v.PushBack(0);
    v.PushBack(1);
    v.PushBack(2);
    const int* const v_data = VectorInternalsAccessor<int>::AllocData(v);

    Vector<int> other(std::move(v));

    assert(other.Size() == 3);
    assert(VectorInternalsAccessor<int>::AllocData(other) == v_data);
    assert(VectorInternalsAccessor<int>::AllocSize(other) == 4);

    assert(v.Size() == 0);
    assert(VectorInternalsAccessor<int>::AllocData(v)
               != VectorInternalsAccessor<int>::AllocData(other));
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 1);
  }
  {
    Vector<int> v1;
    v1.PushBack(5);
    Vector<int> v2;
    v2.PushBack(10);
    v1 = std::move(v2);
    assert(v1[0] == 10);

    Vector<int> v;
    v.PushBack(0);
    v.PushBack(1);
    v.PushBack(2);
    const int* const v_data = VectorInternalsAccessor<int>::AllocData(v);

    Vector<int> other;
    const int* const other_intitial_data =
        VectorInternalsAccessor<int>::AllocData(v);

    other = std::move(v);

    assert(other.Size() == 3);
    assert(VectorInternalsAccessor<int>::AllocData(other) == v_data);
    assert(VectorInternalsAccessor<int>::AllocSize(other) == 4);

    assert(v.Size() == 0);
    assert(VectorInternalsAccessor<int>::AllocData(v)
               != VectorInternalsAccessor<int>::AllocData(other));
    assert(VectorInternalsAccessor<int>::AllocData(v) != other_intitial_data);
    assert(VectorInternalsAccessor<int>::AllocSize(v) == 1);

    auto ret_type = TypeName<decltype(other = std::move(v))>();
    assert(ret_type == std::string(typeid(Vector<int>).name()) + "&");
  }
  std::cout << "[PASS] Move" << std::endl;
#else
  std::cout << "[SKIPPED] Move" << std::endl;
#endif  // SKIP_MOVE

  std::cout << "Finished!" << std::endl;
  return 0;
}
