#include "ArrayThreads.h"
#include <iostream>

void min_max(void* ptr) {
  auto* data = static_cast<ArrayData*>(ptr);

  volatile int& maxElement = data->maxNumber = data->TaskArray[0];
  volatile int& minElement = data->minNumber = data->TaskArray[0];

  for (int i = 1; i < data->numOfElements; ++i) {
    const int& current = data->TaskArray[i];
    if (current > maxElement) maxElement = current;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(ThreadConstants::MIN_MAX_DELAY));
    if (current < minElement) minElement = current;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(ThreadConstants::MIN_MAX_DELAY));
  }

  std::cout << "min element:\t" << minElement << "\n";
  std::cout << "max element:\t" << maxElement << "\n";
}

void average(void* ptr) {
  auto* data = static_cast<ArrayData*>(ptr);

  int sum = data->TaskArray[0];
  for (int i = 1; i < data->numOfElements; ++i) {
    sum += data->TaskArray[i];
    boost::this_thread::sleep_for(boost::chrono::milliseconds(ThreadConstants::AVERAGE_DELAY));
  }
  data->averageNumber = sum / data->numOfElements;

  std::cout << "average element:\t" << data->averageNumber << "\n";
}

void replaceMinMaxWithAverage(ArrayData& data) {
  for (int i = 0; i < data.numOfElements; ++i) {
    if (data.TaskArray[i] == data.maxNumber || data.TaskArray[i] == data.minNumber) {
      data.TaskArray[i] = data.averageNumber;
    }
  }
}
