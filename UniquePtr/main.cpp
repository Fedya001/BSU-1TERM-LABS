#include <iostream>
#include <cassert>
#include <string>
#include <typeinfo>
#include <vector>
#include <utility>

template<typename T>
class UniquePtr {
 public:
  UniquePtr() : pointer_(nullptr) {}

  explicit UniquePtr(T* pointer) : pointer_(pointer) {}

  UniquePtr(UniquePtr&& u_ptr) :
      pointer_(u_ptr.pointer_) {  // move constructor
    u_ptr.pointer_ = nullptr;
  }

  explicit UniquePtr(const UniquePtr&) = delete;

  ~UniquePtr() {
    delete pointer_;
  }

  UniquePtr& operator=(UniquePtr&& u_ptr) {  // move operator
    if (pointer_ != u_ptr.pointer_)
      delete pointer_;
    pointer_ = u_ptr.pointer_;
    u_ptr.pointer_ = nullptr;
    return *this;
  }

  UniquePtr& operator=(const UniquePtr&) = delete;

  T* Get() {
    return pointer_;
  }

  const T* Get() const {
    return pointer_;
  }

  T* Release() {
    T* reference = pointer_;
    pointer_ = nullptr;
    return reference;
  }

  T& operator*() {
    return *pointer_;
  }

  T* operator->() {
    return pointer_;
  }

  const T& operator*() const {
    return *pointer_;
  }

  const T* operator->() const {
    return pointer_;
  }

 private:
  T* pointer_;
};

template<typename T>
class UniquePtr<T[]> {
 public:
  UniquePtr() : pointer_(nullptr) {}

  explicit UniquePtr(T* pointer) : pointer_(pointer) {}

  UniquePtr(UniquePtr&& u_ptr) :
      pointer_(u_ptr.pointer_) {  // move constructor
    u_ptr.pointer_ = nullptr;
  }
  // copy constructor
  explicit UniquePtr(const UniquePtr&) = delete;

  // destructor which pays
  // attention to cleaning data
  ~UniquePtr() {
    delete[] pointer_;
  }

  UniquePtr& operator=(UniquePtr&& u_ptr) {  // move operator
    if (pointer_ != u_ptr.pointer_)
      delete[] pointer_;
    pointer_ = u_ptr.pointer_;
    u_ptr.pointer_ = nullptr;
    return *this;
  }

  // copy operator
  UniquePtr& operator=(const UniquePtr&) = delete;

  T* Get() {
    return pointer_;
  }

  const T* Get() const {
    return pointer_;
  }

  T& operator[](int index) {
    return *(pointer_ + index);
  }

  const T& operator[](int index) const {
    return *(pointer_ + index);
  }

  // The reference is returned
  // to avoid memory leak
  T* Release() {
    T* reference = pointer_;
    pointer_ = nullptr;
    return reference;
  }

  T& operator*() {
    return *pointer_;
  }

  T* operator->() {
    return pointer_;
  }

  const T& operator*() const {
    return *pointer_;
  }

  const T* operator->() const {
    return pointer_;
  }

 private:
  T* pointer_;
};

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
#ifndef SKIP_SINGLE
  {
    UniquePtr<int> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "Pi");
  }
  {
    const UniquePtr<int> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "PKi");
  }
  {
    auto* i = new int(42);
    UniquePtr<int> i_ptr(i);
    assert(i_ptr.Get() == i);
    i_ptr.Release();
    assert(i_ptr.Get() == nullptr);
    delete i;
  }
  {
    auto* i = new int(42);
    UniquePtr<int> i_ptr(i);

    assert(*i_ptr == 42);

    *i = 123;
    assert(*i_ptr == 123);

    assert(TypeName<decltype(*i_ptr)>() == "i&");
    assert(TypeName<decltype(i_ptr.operator->())>() == "Pi");

    const auto& const_i_ptr = i_ptr;
    assert(TypeName<decltype(*const_i_ptr)>() == "const i&");
    assert(TypeName<decltype(const_i_ptr.operator->())>() == "PKi");
  }
  {
    auto* s = new std::string("Hello, ");
    UniquePtr<std::string> s_ptr(s);

    assert(s_ptr->length() == 7);
    assert(*s_ptr == "Hello, ");

    *s_ptr += "world";
    assert(s_ptr->length() == 12);
    assert(*s_ptr == "Hello, world");

    *s += "!";
    assert(s_ptr->length() == 13);
    assert(*s_ptr == "Hello, world!");

    assert(TypeName<decltype(*s_ptr)>() ==
        TypeName<decltype(std::string())>() + "&");
    assert(TypeName<decltype(s_ptr.operator->())>() ==
        "P" + TypeName<decltype(std::string())>());
  }
  {
    auto* s = new std::string("Hello");
    const UniquePtr<std::string> s_ptr(s);

    assert(s_ptr->length() == 5);
    assert(*s_ptr == "Hello");

    *s += "!";
    assert(s_ptr->length() == 6);
    assert(*s_ptr == "Hello!");

    assert(TypeName<decltype(*s_ptr)>()
               == "const " + TypeName<decltype(std::string())>() + "&");
    assert(TypeName<decltype(s_ptr.operator->())>()
               == "PK" + TypeName<decltype(std::string())>());
  }
  {
    auto* s = new std::vector<std::string>({"Hello", "World"});
    const UniquePtr<std::vector<std::string>> s_ptr(s);

    assert(s_ptr->size() == 2);
    assert((*s_ptr)[0] == "Hello");
    assert((*s_ptr)[1] == "World");

    assert(TypeName<decltype((*s_ptr)[0])>()
               == "const " + TypeName<decltype(std::string())>() + "&");
  }
  {
    UniquePtr<std::string> ptr1(new std::string("42"));
    assert(*ptr1 == "42");

    UniquePtr<std::string> ptr2(move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(*ptr2 == "42");
  }
  {
    UniquePtr<std::string> ptr1(new std::string("42"));
    assert(*ptr1 == "42");

    UniquePtr<std::string> ptr2(new std::string("11111"));
    assert(*ptr1 == "42");
    assert(*ptr2 == "11111");

    ptr2 = std::move(ptr1);
    assert(ptr1.Get() == nullptr);
    assert(*ptr2 == "42");

    auto ret_type = TypeName<decltype(ptr2 = std::move(ptr1))>();
    assert(ret_type == std::string(typeid(UniquePtr<std::string>).name()) + "&");
  }
  {
    UniquePtr<std::string> ptr1(new std::string("42"));
    assert(*ptr1 == "42");

    UniquePtr<std::string> ptr2(new std::string("11111"));
    assert(*ptr1 == "42");
    assert(*ptr2 == "11111");

    UniquePtr<std::string> ptr3 = move(ptr2 = std::move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() == nullptr);
    assert(*ptr3 == "42");
  }
  std::cout << "OK: single object" << std::endl;
#else
  std::cout << "Skipped: single object" << std::endl;
#endif  // SKIP_SINGLE

#ifndef SKIP_ARRAY
  {
    UniquePtr<int[]> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "Pi");
  }
  {
    const UniquePtr<int[]> i_ptr;
    assert(i_ptr.Get() == nullptr);
    assert(TypeName<decltype(i_ptr.Get())>() == "PKi");
  }
  {
    auto* i = new int[42];
    UniquePtr<int> i_ptr(i);
    assert(i_ptr.Get() == i);
    i_ptr.
        Release();
    assert(i_ptr.Get() == nullptr);
    delete[]
        i;
  }
  {
    auto* i = new int[4];
    i[0] = 1;
    i[2] = 654321;

    UniquePtr<int[]> i_ptr(i);
    assert(*i_ptr == 1);
    assert(i_ptr[2] == 654321);

    i[2] = 11111;
    assert(i_ptr[2] == 11111);

    i_ptr[2] /= 100;
    assert(i_ptr[2] == 111);

    assert(TypeName<decltype(*i_ptr)>() == "i&");
    assert(TypeName<decltype(i_ptr.operator->())>() == "Pi");
    assert(TypeName<decltype(i_ptr.operator[](2))>() == "i&");
  }
  {
    auto* s = new std::string[4];
    s[0] = "First";
    s[2] = "Hi ";

    UniquePtr<std::string[]> s_ptr(s);
    assert(*s_ptr == "First");
    assert(s_ptr[2] == "Hi ");

    s[2] += "World";
    assert(s_ptr[2] == "Hi World");

    s_ptr[2] += "!!!";
    assert(s_ptr[2] == "Hi World!!!");

    assert(TypeName<decltype(*s_ptr)>()
               == TypeName<decltype(std::string())>() + "&");
    assert(TypeName<decltype(s_ptr.operator->())>()
               == "P" + TypeName<decltype(std::string())>());
    assert(TypeName<decltype(s_ptr.operator[](2))>()
               == TypeName<decltype(std::string())>() + "&");
  }
  {
    auto* i = new int[4];
    i[0] = 1;
    i[2] = 654321;

    const UniquePtr<int[]> i_ptr(i);
    assert(*i_ptr == 1);
    assert(i_ptr[2] == 654321);

    i[2] = 11111;
    assert(i_ptr[2] == 11111);

    assert(TypeName<decltype(*i_ptr)>() == "const i&");
    assert(TypeName<decltype(i_ptr.operator->())>() == "PKi");
    assert(TypeName<decltype(i_ptr.operator[](2))>() == "const i&");
  }
  {
    auto* s = new std::string[4];
    s[0] = "First";
    s[2] = "Hi ";

    const UniquePtr<std::string[]> s_ptr(s);
    assert(*s_ptr == "First");
    assert(s_ptr[2] == "Hi ");

    s[2] += "World";
    assert(s_ptr[2] == "Hi World");

    assert(TypeName<decltype(*s_ptr)>()
               == "const " + TypeName<decltype(std::string())>() + "&");
    assert(TypeName<decltype(s_ptr.operator->())>()
               == "PK" + TypeName<decltype(std::string())>());
    assert(TypeName<decltype(s_ptr.operator[](2))>()
               == "const " + TypeName<decltype(std::string())>() + "&");
  }
  {
    UniquePtr<std::string[]> ptr1(new std::string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<std::string[]> ptr2(move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() != nullptr);
  }
  {
    UniquePtr<std::string[]> ptr1(new std::string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<std::string[]> ptr2(new std::string[2]);
    assert(ptr1.Get() != nullptr);
    assert(ptr2.Get() != nullptr);

    ptr2 = std::move(ptr1);
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() != nullptr);

    auto ret_type = TypeName<decltype(ptr2 = std::move(ptr1))>();
    assert(ret_type == std::string(typeid(UniquePtr<std::string[]>).name()) + "&");
  }
  {
    UniquePtr<std::string[]> ptr1(new std::string[2]);
    assert(ptr1.Get() != nullptr);

    UniquePtr<std::string[]> ptr2(new std::string[2]);
    assert(ptr1.Get() != nullptr);
    assert(ptr2.Get() != nullptr);

    UniquePtr<std::string[]> ptr3 = move(ptr2 = std::move(ptr1));
    assert(ptr1.Get() == nullptr);
    assert(ptr2.Get() == nullptr);
    assert(ptr3.Get() != nullptr);
  }
  std::cout << "OK: array" << std::endl;
#else
  std::cout << "Skipped: array" << std::endl;
#endif  // SKIP_ARRAY

  return 0;
}
