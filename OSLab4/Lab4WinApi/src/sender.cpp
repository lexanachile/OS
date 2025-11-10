#include <iostream>
#include <string>
#include "common.h"
#include "utils.h"
#include "queue_ops.h"

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    std::string filename = argv[1];

    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE hMutex = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
    HANDLE hSemEmpty = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_EMPTY_NAME);
    HANDLE hSemFull = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_FULL_NAME);
    HANDLE hSemReady = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEM_READY_NAME);

    int records_count = getRecordsCount(hFile);
    ReleaseSemaphore(hSemReady, 1, NULL);

    bool running = true;
    while (running) {
        std::cout << "\n1. Send message\n0. Exit\n> ";
        int choice = safeInputInt();

        if (choice == 0) {
            running = false;
        } else if (choice == 1) {
            WaitForSingleObject(hSemEmpty, INFINITE);

            Message msg;
            std::cout << "Message: ";
            std::cin.getline(msg.text, 20);
            if (std::cin.fail()) clearInputBuffer();

            WaitForSingleObject(hMutex, INFINITE);

            writeMessageToQueue(hFile, msg, records_count);

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hSemFull, 1, NULL);
        }
    }

    CloseHandle(hMutex);
    CloseHandle(hSemEmpty);
    CloseHandle(hSemFull);
    CloseHandle(hSemReady);
    CloseHandle(hFile);

    return 0;
}