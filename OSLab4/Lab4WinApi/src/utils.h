#pragma once
#include <string>
#include <windows.h>

void clearInputBuffer();
int safeInputInt();
void logError(const std::string& msg);
int getRecordsCount(HANDLE hFile);