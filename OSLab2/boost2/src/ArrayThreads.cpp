#include "ArrayThreads.h"
#include <iostream>

void min_max(ArrayData* data) {
  int& maxElement = data->maxNumber = data->TaskArray[0];
  int& minElement = data->minNumber = data->TaskArray[0];

  for (int i = 1; i < data->numOfElements; ++i) {
    const int& current = data->TaskArray[i];
    if (current > maxElement)
      maxElement = current;
    std::this_thread::sleep_for(std::chrono::milliseconds(ThreadConstants::MIN_MAX_DELAY));
    if (current < minElement)
      minElement = current;
    std::this_thread::sleep_for(std::chrono::milliseconds(ThreadConstants::MIN_MAX_DELAY));
  }
  std::cout << "min element:\t" << minElement << "\n";
  std::cout << "max element:\t" << maxElement << "\n";
}

void average(ArrayData* data) {
  int& averageElement = data->averageNumber = data->TaskArray[0];
  int i = 1;
  for (; i < data->numOfElements; ++i) {
    const int& current = data->TaskArray[i];
    averageElement += current;
    std::this_thread::sleep_for(std::chrono::milliseconds(ThreadConstants::AVERAGE_DELAY));
  }
  averageElement /= i;
  std::cout << "average element:\t" << averageElement << "\n";
}

void replaceMinMaxWithAverage(ArrayData& data) {
  for (int i = 0; i < data.numOfElements; ++i) {
    if (data.TaskArray[i] == data.maxNumber ||
        data.TaskArray[i] == data.minNumber) {
      data.TaskArray[i] = data.averageNumber;
        }
  }
}