#ifndef ARRAY_PROCESSOR_H
#define ARRAY_PROCESSOR_H

#include <boost/thread.hpp>
#include <boost/chrono.hpp>

namespace ThreadConstants {
const int MIN_MAX_DELAY = 7;
const int AVERAGE_DELAY = 12;
}

struct ArrayData {
  int* TaskArray;
  int numOfElements;
  volatile int maxNumber;
  volatile int minNumber;
  volatile int averageNumber;
};

void min_max(void* ptr);
void average(void* ptr);
void replaceMinMaxWithAverage(ArrayData& data);

#endif
