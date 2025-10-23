#include "worker.h"
#include <iostream>
#include <cstdlib>
#include <boost/chrono.hpp>

void MarkerThread(ThreadParams* params) {
    {
        boost::unique_lock<boost::mutex> lock(params->globalSync->startMutex);
        while (!params->globalSync->isStarted) {
            params->globalSync->startCV.wait(lock);
        }
    }

    srand(params->id);

    while (true) {
        int randomNum = rand();
        int index = randomNum % params->arraySize;

        params->globalSync->csMutex.lock();

        if (params->array[index] == 0) {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
            params->array[index] = params->id;
            boost::this_thread::sleep_for(boost::chrono::milliseconds(5));
            int markedCount = 0;
            for(int i = 0; i < params->arraySize; ++i) {
                if (params->array[i] == params->id) markedCount++;
            }
            params->globalSync->csMutex.unlock();
        }
        else {
            int markedCount = 0;
            for(int i = 0; i < params->arraySize; ++i) {
                if (params->array[i] == params->id) markedCount++;
            }

            std::cout << "Thread ID: " << params->id
                      << ", Marked count: " << markedCount
                      << ", Cannot mark index: " << index << std::endl;

            params->globalSync->csMutex.unlock();

            {
                boost::lock_guard<boost::mutex> lock(params->globalSync->waitAllMutex);
                params->globalSync->pausedThreadsCount++;
            }
            params->globalSync->waitAllCV.notify_one();

            boost::unique_lock<boost::mutex> lock(params->control->mtx);
            while (!params->control->stopSignal && !params->control->continueSignal) {
                params->control->cv.wait(lock);
            }

            if (params->control->stopSignal) {
                params->globalSync->csMutex.lock();
                for (int i = 0; i < params->arraySize; ++i) {
                    if (params->array[i] == params->id) {
                        params->array[i] = 0;
                    }
                }
                params->globalSync->csMutex.unlock();
                return;
            }
            else {
                params->control->continueSignal = false;
            }
        }
    }
}