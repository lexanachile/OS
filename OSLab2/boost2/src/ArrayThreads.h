#ifndef ARRAY_PROCESSOR_H
#define ARRAY_PROCESSOR_H

#include <thread>
#include <chrono>

namespace ThreadConstants {
const int MIN_MAX_DELAY = 7;
const int AVERAGE_DELAY = 12;
}

struct ArrayData {
  int* TaskArray;
  int numOfElements;
  int maxNumber;
  int minNumber;
  int averageNumber;
};

void min_max(ArrayData* data);
void average(ArrayData* data);
void replaceMinMaxWithAverage(ArrayData& data);

#endif