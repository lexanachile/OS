#include "utils.h"
#include "common.h"
#include <iostream>
#include <limits>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int safeInputInt() {
    int value;
    while (true) {
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number: ";
    }
}

void logError(const std::string& msg) {
    std::cerr << "[Error] " << msg << ". Code: " << GetLastError() << std::endl;
}

int getRecordsCount(HANDLE hFile) {
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) return 0;
    return (fileSize - sizeof(QueueHeader)) / sizeof(Message);
}