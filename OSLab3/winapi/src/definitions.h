#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <windows.h>

struct ThreadParams {
  int id;
  int* array;
  int arraySize;
  CRITICAL_SECTION* cs;
  HANDLE hStart;
  HANDLE hCantContinue;
  HANDLE hStop;
  HANDLE hContinue;
  HANDLE hThread;
  bool isActive;
};

#endif