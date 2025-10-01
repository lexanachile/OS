#define BOOST_TEST_MODULE ArrayThreadsTest
#include <boost/test/unit_test.hpp>
#include "C:\about\CppProjects\OS\OSLab2\winapi\src\ArrayThreads.h"

BOOST_AUTO_TEST_CASE(test_min_max_calculation) {
  int testArray[] = {5, 2, 8, 1, 9};
  ArrayData data = {testArray, 5, 0, 0, 0};

  DWORD result = min_max(&data);

  BOOST_CHECK_EQUAL(result, 0);
  BOOST_CHECK_EQUAL(data.minNumber, 1);
  BOOST_CHECK_EQUAL(data.maxNumber, 9);
}

BOOST_AUTO_TEST_CASE(test_average_calculation) {
  int testArray[] = {1, 2, 3, 4, 5};
  ArrayData data = {testArray, 5, 0, 0, 0};

  DWORD result = average(&data);

  BOOST_CHECK_EQUAL(result, 0);
  BOOST_CHECK_EQUAL(data.averageNumber, 3);
}

BOOST_AUTO_TEST_CASE(test_replace_min_max) {
  int testArray[] = {1, 2, 3, 4, 5};
  ArrayData data = {testArray, 5, 5, 1, 3};

  replaceMinMaxWithAverage(data);

  BOOST_CHECK_EQUAL(data.TaskArray[0], 3); // min replaced
  BOOST_CHECK_EQUAL(data.TaskArray[4], 3); // max replaced
  BOOST_CHECK_EQUAL(data.TaskArray[2], 3); // unchanged
}