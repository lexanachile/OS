#include "ArrayThreads.h"
#include <iostream>
#include <thread>

class ArrayProcessor {
public:
  static bool validateInput(int size) {
    return size > 0;
  }
};

int main() {
  ArrayData data = {0};

  std::cout << "Enter array size: ";
  std::cin >> data.numOfElements;

  if (!ArrayProcessor::validateInput(data.numOfElements)) {
    std::cout << "Error: array size must be positive\n";
    return 1;
  }

  data.TaskArray = new int[data.numOfElements];
  std::cout << "Enter " << data.numOfElements << " elements: ";
  for (int i = 0; i < data.numOfElements; ++i) {
    std::cin >> data.TaskArray[i];
  }

  std::thread min_max_thread(min_max, &data);
  std::thread average_thread(average, &data);

  min_max_thread.join();
  average_thread.join();

  replaceMinMaxWithAverage(data);

  std::cout << "Result array: ";
  for (int i = 0; i < data.numOfElements; ++i) {
    std::cout << data.TaskArray[i] << ' ';
  }
  std::cout << std::endl;

  delete[] data.TaskArray;
  return 0;
}