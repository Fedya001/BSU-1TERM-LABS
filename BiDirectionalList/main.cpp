#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <assert.h>

#include "list.h"
#include "tests.h"
#include "testing_framework.h"

void TestAll() {
  TestRunner runner;

  runner.RunTest(TestEmpty, "TestEmpty");
  runner.RunTest(TestAsArrayAndContainerConstructor, "TestAsArrayAndContainerConstructor");
  std::cerr << std::endl;

  runner.RunTest(TestIterator, "TestIterator");
  runner.RunTest(TestIteratorExceptions, "TestIteratorExceptions");
  runner.RunTest(TestConstIterator, "TestConstIterator");
  runner.RunTest(TestConstIteratorExceptions, "TestConstIteratorExceptions");
  runner.RunTest(TestForEachCycle, "TestForEachCycle");
  std::cerr << std::endl;

  runner.RunTest(TestPushPopRandomly, "TestPushPopRandomly");
  runner.RunTest(TestPushPopExceptions, "TestPushPopExceptions");
  std::cerr << std::endl;

  runner.RunTest(TestFindValue, "TestFindValue");
  runner.RunTest(TestFindPredicate, "TestFindPredicate");
  runner.RunTest(TestErase, "TestErase");
  runner.RunTest(TestInsertEraseRandomly, "TestInsertEraseRandomly");
}

int main() {
  TestAll();

  BiDirectionalList<std::vector<int>> list(std::vector<std::vector<int>>{
      {5, 6, 3},
      {0, 1},
      {6, 2}
  });

  //auto it = list.begin();

//  std::vector<int> v{5, 3, 6, 2, 10};
//
//  v.insert(std::find(v.begin(), v.end(), 10), 100500);
//  v.insert(v.begin() + v.size(), 100500);
//
//  for (int item : v) {
//    std::cout << item << ' ';
//  }

  return 0;
}
