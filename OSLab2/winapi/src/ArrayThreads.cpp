#include "ArrayThreads.h"
#include <iostream>

DWORD WINAPI min_max(LPVOID pointer) {
  ArrayData* data = reinterpret_cast<ArrayData*>(pointer);
  volatile int& maxElement = data->maxNumber = data->TaskArray[0];
  volatile int& minElement = data->minNumber = data->TaskArray[0];

  for (int i = 1; i < data->numOfElements; ++i) {
    const int& current = data->TaskArray[i];
    if (current > maxElement)
      maxElement = current;
    Sleep(ThreadConstants::MIN_MAX_DELAY);
    if (current < minElement)
      minElement = current;
    Sleep(ThreadConstants::MIN_MAX_DELAY);
  }

  std::cout << "min element:\t" << minElement << "\n";
  std::cout << "max element:\t" << maxElement << "\n";
  return 0;
}

DWORD WINAPI average(LPVOID pointer) {
  ArrayData* data = reinterpret_cast<ArrayData*>(pointer);
  volatile int& averageElement = data->averageNumber = data->TaskArray[0];
  int i = 1;

  for (; i < data->numOfElements; ++i) {
    const int& current = data->TaskArray[i];
    averageElement += current;
    Sleep(ThreadConstants::AVERAGE_DELAY);
  }

  averageElement /= i;
  std::cout << "average element:\t" << averageElement << "\n";
  return 0;
}

void replaceMinMaxWithAverage(ArrayData& data) {
  for (int i = 0; i < data.numOfElements; ++i) {
    if (data.TaskArray[i] == data.maxNumber ||
        data.TaskArray[i] == data.minNumber) {
      data.TaskArray[i] = data.averageNumber;
        }
  }
}
