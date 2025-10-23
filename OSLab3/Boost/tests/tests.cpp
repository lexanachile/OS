#include <iostream>
#include <boost/thread.hpp>
#include <cassert>
#include "definitions.h"
#include "worker.h"

void TestMutex() {
    boost::mutex mtx;
    mtx.lock();
    mtx.unlock();
    std::cout << "TestMutex passed." << std::endl;
}

void TestConditionVariable() {
    boost::mutex mtx;
    boost::condition_variable cv;
    bool ready = false;

    boost::thread t([&]() {
        boost::unique_lock<boost::mutex> lock(mtx);
        while (!ready) {
            cv.wait(lock);
        }
    });

    {
        boost::lock_guard<boost::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
    t.join();

    std::cout << "TestConditionVariable passed." << std::endl;
}

void TestThreadLogic() {
    int array[5] = {0, 0, 0, 0, 0};

    GlobalSync globalSync;
    globalSync.isStarted = false;
    globalSync.pausedThreadsCount = 0;

    ThreadParams* params = new ThreadParams;
    params->id = 1;
    params->array = array;
    params->arraySize = 5;
    params->globalSync = &globalSync;
    params->control = new ThreadControl;
    params->control->stopSignal = false;
    params->control->continueSignal = false;
    params->isActive = true;

    params->threadObj = new boost::thread(MarkerThread, params);

    {
        boost::lock_guard<boost::mutex> lock(globalSync.startMutex);
        globalSync.isStarted = true;
    }
    globalSync.startCV.notify_all();

    {
        boost::unique_lock<boost::mutex> lock(globalSync.waitAllMutex);
        while (globalSync.pausedThreadsCount < 1) {
            globalSync.waitAllCV.wait(lock);
        }
    }

    {
        boost::lock_guard<boost::mutex> lk(params->control->mtx);
        params->control->stopSignal = true;
    }
    params->control->cv.notify_one();
    params->threadObj->join();

    delete params->threadObj;
    delete params->control;
    delete params;

    std::cout << "TestThreadLogic passed." << std::endl;
}

int main() {
    TestMutex();
    TestConditionVariable();
    TestThreadLogic();
    std::cout << "All tests passed." << std::endl;
    return 0;
}