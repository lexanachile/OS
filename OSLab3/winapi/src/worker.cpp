#include "worker.h"
#include <iostream>
#include <vector>

DWORD WINAPI MarkerThread(LPVOID lpParam) {
  ThreadParams *params = (ThreadParams *)lpParam;

  WaitForSingleObject(params->hStart, INFINITE);

  srand(params->id);

  int markedCount = 0;
  while (true) {
    int index = rand() % params->arraySize;

    EnterCriticalSection(params->cs);

    if (params->array[index] == 0) {
      Sleep(5);
      params->array[index] = params->id;
      Sleep(5);
      markedCount++;
      LeaveCriticalSection(params->cs);
    } else {
      std::cout << "Thread ID: " << params->id
                << ", Marked count: " << markedCount
                << ", Cannot mark index: " << index << std::endl;
      LeaveCriticalSection(params->cs);

      SetEvent(params->hCantContinue);

      HANDLE waitEvents[2] = {
        params->hStop, params->hContinue
      };
      DWORD waitResult = WaitForMultipleObjects(2, waitEvents, FALSE, INFINITE);

      if (waitResult == WAIT_OBJECT_0) {
        EnterCriticalSection(params->cs);
        for (int i = 0; i < params->arraySize; ++i) {
          if (params->array[i] == params->id) {
            params->array[i] = 0;
          }
        }
        LeaveCriticalSection(params->cs);
        return 0;
      } else {
        ResetEvent(params->hCantContinue);
      }
    }
  }
  return 0;
}