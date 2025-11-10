#include <iostream>
#include <vector>
#include <string>
#include "common.h"
#include "utils.h"
#include "queue_ops.h"

int main() {
    std::string filename;
    int records_count;
    int senders_count;

    std::cout << "Enter binary filename: ";
    std::cin >> filename;
    clearInputBuffer();
    filename += ".bin";

    while (true) {
        std::cout << "Enter number of records: ";
        records_count = safeInputInt();
        if (records_count > 0) break;
    }

    if (!initQueueFile(filename, records_count)) {
        logError("Failed to create file");
        return 1;
    }

    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE hMutex = CreateMutexA(NULL, FALSE, MUTEX_NAME);
    HANDLE hSemEmpty = CreateSemaphoreA(NULL, records_count, records_count, SEM_EMPTY_NAME);
    HANDLE hSemFull = CreateSemaphoreA(NULL, 0, records_count, SEM_FULL_NAME);
    HANDLE hSemReady = CreateSemaphoreA(NULL, 0, 100, SEM_READY_NAME);

    while (true) {
        std::cout << "Enter number of Senders: ";
        senders_count = safeInputInt();
        if (senders_count > 0) break;
    }

    std::vector<STARTUPINFOA> si(senders_count);
    std::vector<PROCESS_INFORMATION> pi(senders_count);

    for (int i = 0; i < senders_count; ++i) {
        ZeroMemory(&si[i], sizeof(si[i]));
        si[i].cb = sizeof(si[i]);
        ZeroMemory(&pi[i], sizeof(pi[i]));

        std::string cmdArgs = "Sender.exe " + filename;
        std::vector<char> mutableArgs(cmdArgs.begin(), cmdArgs.end());
        mutableArgs.push_back(0);

        CreateProcessA(NULL, mutableArgs.data(), NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i]);
    }

    for (int i = 0; i < senders_count; ++i) {
        WaitForSingleObject(hSemReady, INFINITE);
    }

    bool running = true;
    while (running) {
        std::cout << "\n1. Read message\n0. Exit\n> ";
        int choice = safeInputInt();

        if (choice == 0) {
            running = false;
        } else if (choice == 1) {
            WaitForSingleObject(hSemFull, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);

            Message msg = readMessageFromQueue(hFile, records_count);
            std::cout << "Received: " << msg.text << std::endl;

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hSemEmpty, 1, NULL);
        }
    }

    for (int i = 0; i < senders_count; ++i) {
        if (pi[i].hProcess) {
            TerminateProcess(pi[i].hProcess, 0);
            CloseHandle(pi[i].hProcess);
            CloseHandle(pi[i].hThread);
        }
    }

    CloseHandle(hMutex);
    CloseHandle(hSemEmpty);
    CloseHandle(hSemFull);
    CloseHandle(hSemReady);
    CloseHandle(hFile);

    return 0;
}