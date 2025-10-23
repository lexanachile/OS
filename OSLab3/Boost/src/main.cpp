#include <iostream>
#include <vector>
#include <boost/thread.hpp>
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

    GlobalSync globalSync;
    globalSync.isStarted = false;
    globalSync.pausedThreadsCount = 0;

    std::vector<ThreadParams*> threadsData;

    for (int i = 0; i < threadCount; ++i) {
        ThreadParams* params = new ThreadParams;
        params->id = i + 1;
        params->array = sharedArray;
        params->arraySize = arraySize;
        params->globalSync = &globalSync;
        params->control = new ThreadControl;
        params->control->stopSignal = false;
        params->control->continueSignal = false;
        params->isActive = true;

        params->threadObj = new boost::thread(MarkerThread, params);
        threadsData.push_back(params);
    }

    {
        boost::lock_guard<boost::mutex> lock(globalSync.startMutex);
        globalSync.isStarted = true;
    }
    globalSync.startCV.notify_all();

    int activeThreads = threadCount;

    while (activeThreads > 0) {
        {
            boost::unique_lock<boost::mutex> lock(globalSync.waitAllMutex);
            while (globalSync.pausedThreadsCount < activeThreads) {
                globalSync.waitAllCV.wait(lock);
            }
        }

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
            {
                boost::lock_guard<boost::mutex> lk(threadsData[stopIndex]->control->mtx);
                threadsData[stopIndex]->control->stopSignal = true;
            }
            threadsData[stopIndex]->control->cv.notify_one();

            threadsData[stopIndex]->threadObj->join();
            threadsData[stopIndex]->isActive = false;

            {
                boost::lock_guard<boost::mutex> lock(globalSync.waitAllMutex);
                globalSync.pausedThreadsCount--;
            }
            activeThreads--;

            std::cout << "Array content after thread termination: ";
            for (int i = 0; i < arraySize; ++i) {
                std::cout << sharedArray[i] << " ";
            }
            std::cout << std::endl;
        }

        {
            boost::lock_guard<boost::mutex> lock(globalSync.waitAllMutex);
            globalSync.pausedThreadsCount = 0;
        }

        for (auto params : threadsData) {
            if (params->isActive) {
                {
                    boost::lock_guard<boost::mutex> lk(params->control->mtx);
                    params->control->continueSignal = true;
                }
                params->control->cv.notify_one();
            }
        }
    }

    std::cout << "All threads finished." << std::endl;

    for (auto params : threadsData) {
        delete params->threadObj;
        delete params->control;
        delete params;
    }
    delete[] sharedArray;

    return 0;
}