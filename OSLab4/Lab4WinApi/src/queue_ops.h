#pragma once
#include <windows.h>
#include <string>
#include "common.h"

bool initQueueFile(const std::string& filename, int records_count);
Message readMessageFromQueue(HANDLE hFile, int records_count);
void writeMessageToQueue(HANDLE hFile, const Message& msg, int records_count);
QueueHeader readHeader(HANDLE hFile);