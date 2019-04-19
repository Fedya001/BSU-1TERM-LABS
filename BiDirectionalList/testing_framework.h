//
// Created by user on 24.12.2018.
//

#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <sstream>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s);

template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint = {});

void Assert(bool b, const std::string& hint);

class TestRunner {
 public:
  template<class TestFunc>
  void RunTest(TestFunc func, const std::string& test_name);
  ~TestRunner();

 private:
  int fail_count = 0;
};

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const std::string& hint) {
  if (t != u) {
    std::ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
      os << " hint: " << hint;
    }
    throw std::runtime_error(os.str());
  }
}

template<class TestFunc>
void TestRunner::RunTest(TestFunc func, const std::string& test_name) {
  try {
    func();
    std::cerr << test_name << " OK" << std::endl;
  } catch (std::exception& e) {
    ++fail_count;
    std::cerr << test_name << " fail: " << e.what() << std::endl;
  } catch (...) {
    ++fail_count;
    std::cerr << "Unknown exception caught" << std::endl;
  }
}

#endif //TEST_RUNNER_H