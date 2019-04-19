//
// Created by user on 25.12.2018.
//

#include <vector>
#include <random>
#include <chrono>
#include <deque>
#include <list>
#include <algorithm>

#include "testing_framework.h"
#include "list.h"
#include "tests.h"

// struct for testing

struct Point {
  int x;
  int y;
  bool operator==(const Point& other) const {
    return (x == other.x && y == other.y);
  }
  bool operator!=(const Point& other) const {
    return !(*this == other);
  }
};

std::ostream& operator<<(std::ostream& output, const Point& point) {
  output << '{' << point.x << ", " << point.y << '}' << std::endl;
  return output;
}

class RandomIntGenerator {
 public:
  RandomIntGenerator(int left, int right, long long seed =
  std::chrono::duration_cast<std::chrono::microseconds>
      (std::chrono::system_clock::now().time_since_epoch()).count())
      : generator(seed), distribution(left, right) {}

  int NextInt() {
    return distribution(generator);
  }
 private:
  std::mt19937 generator;
  std::uniform_int_distribution<> distribution;
};

enum class PushPopMethods {
  PUSH_FRONT,
  PUSH_BACK,
  POP_FRONT,
  POP_BACK
};

enum class InsertEraseMethods {
  INSERT_BEFORE,
  INSERT_AFTER,
  ERASE,
};

template<typename InputIter, typename T>
std::vector<T> ContainerAsArray(InputIter first, InputIter last) {
  std::vector<T> array;
  for (auto iterator = first; iterator != last; ++iterator) {
    array.push_back(*iterator);
  }
  return array;
}

void TestEmpty() {
  BiDirectionalList<int> list;
  Assert(list.IsEmpty(), "list should be empty");
  list.PushBack(5);
  list.PopBack();
  Assert(list.IsEmpty(), "PushBack, PopBack by const T& -> expected empty");
  list.PushFront(42);
  list.PopFront();
  Assert(list.IsEmpty(), "PushFront, PopFront by const T& -> expected empty");

  int x = 42;
  list.PushBack(std::move(x));
  list.PopBack();
  Assert(list.IsEmpty(), "PushBack, PopBack by && -> expected empty");
  AssertEqual(x, 0, "Temporary variable not cleaned");
  x = 1234;
  list.PushFront(std::move(x));
  list.PopFront();
  Assert(list.IsEmpty(), "PushFront, PopFront by && -> expected empty");
  AssertEqual(x, 0, "Temporary variable not cleaned");

  list.PushBack(42);
  list.PopFront();
  Assert(list.IsEmpty(), "PushBack, PopFront -> expected empty");

  list.PushFront(42);
  list.PopBack();
  Assert(list.IsEmpty(), "PushFront, PopBack -> expected empty");
}

void TestAsArrayAndContainerConstructor() {
  const std::string constructor_fault_toast = "list constructor doesn't work";
  const std::string as_array_fault_toast = "list .AsArray doesn't work";

  std::vector<float> float_vec{1.5, 234.0, 0.55, -5.32};
  BiDirectionalList<float> float_list(float_vec);
  AssertEqual(float_list.Size(), 4u, constructor_fault_toast);
  AssertEqual(float_list.AsArray(), float_vec, as_array_fault_toast);

  std::vector<int> int_vec{5};
  BiDirectionalList<int> int_list(int_vec);
  AssertEqual(int_list.Size(), 1u, constructor_fault_toast);
  AssertEqual(int_list.AsArray(), int_vec, as_array_fault_toast);

  std::vector<Point> point_vec{{4, 2}, {0, 0}};
  BiDirectionalList<Point> point_list(point_vec);
  AssertEqual(point_list.Size(), 2u, constructor_fault_toast);
  AssertEqual(point_list.AsArray(), point_vec, as_array_fault_toast);

  AssertEqual(BiDirectionalList<int>{}.Size(), 0u, constructor_fault_toast);
  AssertEqual(BiDirectionalList<int>{}.AsArray(), std::vector<int>{}, as_array_fault_toast);
};

void TestIterator() {
  BiDirectionalList<int> list(std::vector<int>{4, 5, 2, 0, 42, 24});
  AssertEqual(*list.begin(), 4, "Wrong begin iterator");
  AssertEqual(*(--list.end()), 24, "Wrong end iterator");

  auto it = list.begin();
  auto another_it = ++it;
  Assert(another_it == it, "Postfix increment works wrong");
  AssertEqual(*another_it, 5, "operator* fault");

  it = another_it;
  another_it = it++;
  Assert(another_it != it, "Prefix increment works wrong");
  AssertEqual(*another_it, 5, "operator* fault");

  it = another_it;
  another_it = it--;
  Assert(*another_it == 5, "Postfix decrement works wrong");

  it = another_it;
  another_it = --it;
  Assert(*another_it == 4, "Prefix decrement works wrong");

  BiDirectionalList<Point> point_list(std::vector<Point>{{1, 2}, {10, 66}, {-6, 42}});
  auto point_it = point_list.begin();
  ++point_it;
  AssertEqual(*point_it, Point{10, 66}, "operator-> works wrong");

  BiDirectionalList<Point> another(std::vector<Point>{{100500, 100500}});
  try {
    point_it = another.begin();
    throw std::runtime_error("assignment iterator from another list shouldn't be allowed");
  } catch (const std::invalid_argument& ex) {
    // everything work's fine
  }
};

void TestIteratorExceptions() {
  BiDirectionalList<int> list(std::vector<int>{5});
  auto it = list.end();
  try {
    *it;
    throw std::runtime_error("operator* from end() should throw an exception");
  } catch (const std::invalid_argument& ex) {
    // everything work correct
  }

  --it;
  AssertEqual(*it, 5, "Decrement from end() should return valid iterator");

  BiDirectionalList<Point> point_list(std::vector<Point>{{6, 7}, {0, -2}});
  auto point_it = point_list.end();
  try {
    *point_it;
    throw std::runtime_error("operator* from end() should throw an exception");
  } catch (const std::invalid_argument& ex) {
    // everything work correct
  }

  --point_it;
  AssertEqual(point_it->y, -2, "");
};

void TestConstIterator() {
  const BiDirectionalList<int> list(std::vector<int>{4, 5, 2, 0, 42, 24});

  Assert(typeid(list.begin()) == typeid(BiDirectionalList<int>::ConstIterator),
         "begin() should return ConstIterator for const list");
  Assert(typeid(list.end()) == typeid(BiDirectionalList<int>::ConstIterator),
         "end() should return ConstIterator for const list");

  AssertEqual(*list.begin(), 4, "Wrong begin iterator");
  AssertEqual(*(--list.end()), 24, "Wrong end iterator");

  auto it = list.begin();
  auto another_it = ++it;
  Assert(another_it == it, "Postfix increment works wrong");
  AssertEqual(*another_it, 5, "operator* fault");

  it = another_it;
  another_it = it++;
  Assert(another_it != it, "Prefix increment works wrong");
  AssertEqual(*another_it, 5, "operator* fault");

  it = another_it;
  another_it = it--;
  Assert(*another_it == 5, "Postfix decrement works wrong");

  it = another_it;
  another_it = --it;
  Assert(*another_it == 4, "Prefix decrement works wrong");

  BiDirectionalList<Point> point_list(std::vector<Point>{{1, 2}, {10, 66}, {-6, 42}});
  auto point_it = point_list.begin();
  ++point_it;
  AssertEqual(*point_it, Point{10, 66}, "operator-> works wrong");

  BiDirectionalList<Point> another(std::vector<Point>{{100500, 100500}});
  try {
    point_it = another.begin();
    throw std::runtime_error("assignment iterator from another list shouldn't be allowed");
  } catch (const std::invalid_argument& ex) {
    // everything work's fine
  }
}

void TestConstIteratorExceptions() {
  BiDirectionalList<int> list(std::vector<int>{5});
  auto it = list.end();
  try {
    *it;
    throw std::runtime_error("operator* from end() should throw an exception");
  } catch (const std::invalid_argument& ex) {
    // everything work correct
  }

  --it;
  AssertEqual(*it, 5, "Decrement from end() should return valid iterator");

  BiDirectionalList<Point> point_list(std::vector<Point>{{6, 7}, {0, -2}});
  auto point_it = point_list.end();
  try {
    *point_it;
    throw std::runtime_error("operator* from end() should throw an exception");
  } catch (const std::invalid_argument& ex) {
    // everything work correct
  }

  --point_it;
  AssertEqual(point_it->y, -2, "");
}

void TestForEachCycle() {
  BiDirectionalList<uint8_t> list(std::vector<uint8_t>{0, 32, 123, 23, 253});
  std::vector<uint8_t> result;
  for (const auto& item : list) {
    result.push_back(item);
  }
  AssertEqual(list.AsArray(), result);
};

void TestPushPopRandomly() {
  RandomIntGenerator action_generator(0, 3);
  RandomIntGenerator value_generator(-1'000'000'000, 1'000'000'000);

  BiDirectionalList<int> list;
  std::deque<int> deq;

  for (int i = 0; i < 200; ++i) {
    int value = value_generator.NextInt();
    if (action_generator.NextInt() % 2 == 0) {
      list.PushFront(value);
      deq.push_front(value);
    } else {
      list.PushBack(value);
      deq.push_back(value);
    }
  }

  for (int i = 0; i < 1'000; ++i) {
    int value = value_generator.NextInt();

    PushPopMethods method_name = static_cast<PushPopMethods>(action_generator.NextInt());
    switch (method_name) {
      case PushPopMethods::PUSH_FRONT:list.PushFront(value);
        deq.push_front(value);
        break;
      case PushPopMethods::PUSH_BACK:list.PushBack(value);
        deq.push_back(value);
        break;
      case PushPopMethods::POP_FRONT:list.PopFront();
        deq.pop_front();
        break;
      case PushPopMethods::POP_BACK:list.PopBack();
        deq.pop_back();
        break;
      default:throw std::runtime_error("Action generator provided wrong number.");
    }
    AssertEqual(list.AsArray(),
                ContainerAsArray<std::deque<int>::iterator, int>(deq.begin(), deq.end()),
                "deque and BiDirectionalList differ");
    AssertEqual(list.Size(), deq.size(), "deque and list sizes must be equal");
  }
}

void TestPushPopExceptions() {
  int list_size = 0;
  RandomIntGenerator action_generator(0, 1);
  RandomIntGenerator value_generator(-1'000'000'000, 1'000'000'000);

  BiDirectionalList<int> list;
  for (int i = 0; i < 10'000; ++i) {
    int value = value_generator.NextInt();

    PushPopMethods method_name = static_cast<PushPopMethods>(action_generator.NextInt());
    switch (method_name) {
      case PushPopMethods::PUSH_FRONT:list.PushFront(value);
        ++list_size;
        break;
      case PushPopMethods::PUSH_BACK:list.PushBack(value);
        ++list_size;
        break;
      case PushPopMethods::POP_FRONT:
        try {
          list.PopFront();
          Assert(list_size != 0, "exception should be "
                                 "thrown when popping from empty list");
          --list_size;
        } catch (const std::runtime_error& ex) {
          Assert(list_size == 0, "exception should not be "
                                 "thrown when popping from non-empty list");
        }
        break;
      case PushPopMethods::POP_BACK:
        try {
          list.PopBack();
          Assert(list_size != 0, "exception should be "
                                 "thrown when popping from empty list");
          --list_size;
        } catch (const std::runtime_error& ex) {
          Assert(list_size == 0, "exception should not be "
                                 "thrown when popping from non-empty list");
        }
        break;
      default:throw std::runtime_error("Action generator provided wrong number.");
    }
  }
}

template<typename Container>
void FillIntEqualContainers(BiDirectionalList<int>& best_list, Container& correct_list,
                            RandomIntGenerator& generator, int count) {
  for (int i = 0; i < count; ++i) {
    int random_value = generator.NextInt();
    correct_list.push_back(random_value);
    best_list.PushBack(random_value);
  }
}

void TestFindValue() {
  const std::string fault_toast = "Find method works wrong";
  RandomIntGenerator generator(1, 200);

  BiDirectionalList<int> best_list;
  std::list<int> correct_list;
  FillIntEqualContainers(best_list, correct_list, generator, 200);

  for (int i = 0; i < 1'000; ++i) {
    int searching_value = generator.NextInt();
    auto best_list_pos = best_list.Find(searching_value);
    auto correct_list_pos = std::find(correct_list.begin(), correct_list.end(), searching_value);

    if (best_list_pos != best_list.end() && correct_list_pos != correct_list.end()) {
      AssertEqual(*best_list_pos, *correct_list_pos, fault_toast);
    } else {
      if (best_list_pos != best_list.end() || correct_list_pos != correct_list.end()) {
        throw std::runtime_error(fault_toast);
      }
    }
  }
};

void TestFindPredicate() {
  std::vector<std::function<bool(int)>> lambdas = {
      // IsOdd
      [](int x) {
        return x % 2 == 1;
      },
      // IsPrime
      [](int x) {
        for (int i = 2; i * i <= x; ++i) {
          if (x % i == 0) {
            return false;
          }
        }
        return true;
      },
      // IsPalindrome
      [](int x) {
        std::string non_reversed = std::to_string(x);
        std::string reversed = non_reversed;
        std::reverse(reversed.begin(), reversed.end());
        return non_reversed == reversed;
      },
  };

  const std::string fault_toast = "Find method works wrong";
  RandomIntGenerator value_generator(1, 200);
  RandomIntGenerator action_generator(0, 2);

  BiDirectionalList<int> best_list;
  std::list<int> correct_list;
  FillIntEqualContainers(best_list, correct_list, value_generator, 200);

  for (int i = 0; i < 1'000; ++i) {
    int lambda_index = action_generator.NextInt();

    auto best_list_pos = best_list.Find(lambdas[lambda_index]);
    auto correct_list_pos = std::find_if(correct_list.begin(), correct_list.end(), lambdas[lambda_index]);

    if (best_list_pos != best_list.end() && correct_list_pos != correct_list.end()) {
      AssertEqual(*best_list_pos, *correct_list_pos, fault_toast);
    } else {
      if (best_list_pos != best_list.end() || correct_list_pos != correct_list.end()) {
        throw std::runtime_error(fault_toast);
      }
    }
  }
}

void TestErase() {
  const std::string fault_toast = "Erase method works wrong";
  RandomIntGenerator generator(1, 500);

  BiDirectionalList<int> best_list;
  std::list<int> correct_list;
  FillIntEqualContainers(best_list, correct_list, generator, 800);

  // Bounding begin() and end() iterators
  for (int i = 0; i < 10; ++i) {
    best_list.Erase(best_list.begin());
    correct_list.erase(correct_list.begin());
    AssertEqual(best_list.AsArray(),
                ContainerAsArray<std::list<int>::iterator, int>(correct_list.begin(), correct_list.end()),
                fault_toast);
  }
  for (int i = 0; i < 10; ++i) {
    best_list.Erase(--best_list.end());
    correct_list.erase(--correct_list.end());
    AssertEqual(best_list.AsArray(),
                ContainerAsArray<std::list<int>::iterator, int>(correct_list.begin(), correct_list.end()),
                fault_toast);
  }

  // exceptions
  try {
    best_list.Erase(best_list.end());
    throw std::runtime_error("erase should throw an exception after invocation from end()");
  } catch (const std::invalid_argument& ex) {
    // everything is correct
  }

  // single element

  BiDirectionalList<int> single_element(std::vector<int>{4});
  single_element.Erase(single_element.begin());
  AssertEqual(single_element.AsArray(), std::vector<int>{}, "wrong single element erase");

  //random

  for (int i = 0; i < 600; ++i) {
    int erasing_value = generator.NextInt();
    auto best_list_pos = best_list.Find(erasing_value);
    auto correct_list_pos = std::find(correct_list.begin(), correct_list.end(), erasing_value);

    if (best_list_pos != best_list.end()) {
      best_list.Erase(best_list_pos);
      correct_list.erase(correct_list_pos);
      AssertEqual(best_list.AsArray(),
                  ContainerAsArray<std::list<int>::iterator, int>(correct_list.begin(), correct_list.end()),
                  fault_toast);
    }
  }
}

void TestInsertEraseRandomly() {
  RandomIntGenerator value_generator(-1'000'000'000, 1'000'000'000);

  BiDirectionalList<int> list;
  std::vector<int> vec;
  FillIntEqualContainers(list, vec, value_generator, 200);

  RandomIntGenerator action_generator(0, 2);
  for (int i = 0; i < 1'000; ++i) {
    int iterator_index = abs(value_generator.NextInt()) % vec.size();
    auto iterator = list.Find(vec[iterator_index]);

    InsertEraseMethods method_name = static_cast<InsertEraseMethods>(action_generator.NextInt());
    switch (method_name) {
      case InsertEraseMethods::INSERT_AFTER: {
        int value = value_generator.NextInt();
        list.InsertAfter(iterator, value);
        vec.insert(vec.begin() + iterator_index + 1, value);
        break;
      }
      case InsertEraseMethods::INSERT_BEFORE: {
        int value = value_generator.NextInt();
        list.InsertBefore(iterator, value);
        vec.insert(vec.begin() + iterator_index, value);
        break;
      }
      case InsertEraseMethods::ERASE: {
        list.Erase(iterator);
        vec.erase(vec.begin() + iterator_index);
        break;
      }
      default: throw std::runtime_error("Action generator provided wrong number.");
    }
    AssertEqual(list.AsArray(), ContainerAsArray<std::vector<int>::iterator, int>(vec.begin(), vec.end()), "Insert method work's wrong");
  }
}