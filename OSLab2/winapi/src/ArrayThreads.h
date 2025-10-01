#ifndef ARRAY_PROCESSOR_H
#define ARRAY_PROCESSOR_H

#include <windows.h>

namespace ThreadConstants {
const DWORD MIN_MAX_DELAY = 7;
const DWORD AVERAGE_DELAY = 12;
}

struct ArrayData {
  int* TaskArray;
  int numOfElements;
  volatile int maxNumber;
  volatile int minNumber;
  volatile int averageNumber;
};

DWORD WINAPI min_max(LPVOID pointer);
DWORD WINAPI average(LPVOID pointer);
void replaceMinMaxWithAverage(ArrayData& data);

#endif
