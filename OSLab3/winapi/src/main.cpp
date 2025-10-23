#include <iostream>
#include <vector>
#include <windows.h>
#include "definitions.h"
#include "worker.h"

int main() {
    int arraySize;
    std::cout << "Enter array size: ";
    std::cin >> arraySize;

    int* sharedArray = new int[arraySize];
    for (int i = 0; i < arraySize; ++i) {
        sharedArray[i] = 0;
    }

    int threadCount;
    std::cout << "Enter number of marker threads: ";
    std::cin >> threadCount;

    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    HANDLE hStart = CreateEvent(NULL, TRUE, FALSE, NULL);

    std::vector<ThreadParams*> threadsData;

    for (int i = 0; i < threadCount; ++i) {
        ThreadParams* params = new ThreadParams;
        params->id = i + 1;
        params->array = sharedArray;
        params->arraySize = arraySize;
        params->cs = &cs;
        params->hStart = hStart;
        params->hCantContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
        params->hStop = CreateEvent(NULL, FALSE, FALSE, NULL);
        params->hContinue = CreateEvent(NULL, FALSE, FALSE, NULL);
        params->isActive = true;

        params->hThread = CreateThread(NULL, 0, MarkerThread, params, 0, NULL);
        threadsData.push_back(params);
    }

    SetEvent(hStart);

    int activeThreads = threadCount;

    while (activeThreads > 0) {
        std::vector<HANDLE> activeWaitHandles;
        for (auto params : threadsData) {
            if (params->isActive) {
                activeWaitHandles.push_back(params->hCantContinue);
            }
        }

        WaitForMultipleObjects((DWORD)activeWaitHandles.size(), activeWaitHandles.data(), TRUE, INFINITE);

        std::cout << "Array content: ";
        for (int i = 0; i < arraySize; ++i) {
            std::cout << sharedArray[i] << " ";
        }
        std::cout << std::endl;

        int stopIndex = -1;
        while (true) {
            std::cout << "Available threads: ";
            for (const auto* params : threadsData) {
                if (params->isActive) {
                    std::cout << params->id << " ";
                }
            }
            std::cout << std::endl;

            int stopId;
            std::cout << "Enter thread ID to stop: ";
            std::cin >> stopId;

            stopIndex = -1;
            for(size_t i = 0; i < threadsData.size(); ++i) {
                if(threadsData[i]->id == stopId && threadsData[i]->isActive) {
                    stopIndex = i;
                    break;
                }
            }

            if (stopIndex != -1) {
                break;
            } else {
                std::cout << "Invalid thread ID or thread already stopped. Please try again." << std::endl;
            }
        }

        if (stopIndex != -1) {
            SetEvent(threadsData[stopIndex]->hStop);
            WaitForSingleObject(threadsData[stopIndex]->hThread, INFINITE);

            threadsData[stopIndex]->isActive = false;
            --activeThreads;

            std::cout << "Array content after thread termination: ";
            for (int i = 0; i < arraySize; ++i) {
                std::cout << sharedArray[i] << " ";
            }
            std::cout << std::endl;
        }

        for (auto params : threadsData) {
            if (params->isActive) {
                ResetEvent(params->hCantContinue);
                SetEvent(params->hContinue);
            }
        }
    }

    std::cout << "All threads finished." << std::endl;

    for (auto params : threadsData) {
        CloseHandle(params->hThread);
        CloseHandle(params->hCantContinue);
        CloseHandle(params->hStop);
        CloseHandle(params->hContinue);
        delete params;
    }
    CloseHandle(hStart);
    DeleteCriticalSection(&cs);
    delete[] sharedArray;

    return 0;
}