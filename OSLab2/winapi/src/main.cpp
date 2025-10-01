#include "ArrayThreads.h"
#include <windows.h>
#include <iostream>

class ArrayProcessor {
public:
  static bool validateInput(int size) {
    return size > 0;
  }

  static void handleThreadError(HANDLE h1, HANDLE h2) {
    std::cout << "Thread creation error\n";
    if (h1) CloseHandle(h1);
    if (h2) CloseHandle(h2);
  }
};

int main() {
  HANDLE hmin_max = NULL;
  HANDLE haverage = NULL;
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

  hmin_max = CreateThread(NULL, 0, min_max, &data, 0, NULL);
  haverage = CreateThread(NULL, 0, average, &data, 0, NULL);

  if (NULL == hmin_max || NULL == haverage) {
    ArrayProcessor::handleThreadError(hmin_max, haverage);
    delete[] data.TaskArray;
    return GetLastError();
  }

  WaitForSingleObject(hmin_max, INFINITE);
  WaitForSingleObject(haverage, INFINITE);
  CloseHandle(hmin_max);
  CloseHandle(haverage);

  replaceMinMaxWithAverage(data);

  std::cout << "Result array: ";
  for (int i = 0; i < data.numOfElements; ++i) {
    std::cout << data.TaskArray[i] << ' ';
  }
  std::cout << std::endl;

  delete[] data.TaskArray;
  return 0;
}
