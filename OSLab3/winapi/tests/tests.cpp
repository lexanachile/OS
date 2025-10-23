#include <iostream>
#include <windows.h>
#include <cassert>
#include "definitions.h"
#include "worker.h"

void TestCriticalSection() {
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    EnterCriticalSection(&cs);
    LeaveCriticalSection(&cs);
    DeleteCriticalSection(&cs);
    std::cout << "TestCriticalSection passed." << std::endl;
}

void TestEvents() {
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    assert(hEvent != NULL);

    DWORD res = WaitForSingleObject(hEvent, 0);
    assert(res == WAIT_TIMEOUT);

    SetEvent(hEvent);
    res = WaitForSingleObject(hEvent, 0);
    assert(res == WAIT_OBJECT_0);

    ResetEvent(hEvent);
    res = WaitForSingleObject(hEvent, 0);
    assert(res == WAIT_TIMEOUT);

    CloseHandle(hEvent);
    std::cout << "TestEvents passed." << std::endl;
}

void TestThreadLogic() {
    int array[5] = {0, 0, 0, 0, 0};
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    HANDLE hStart = CreateEvent(NULL, TRUE, FALSE, NULL);

    ThreadParams* params = new ThreadParams;
    params->id = 1;
    params->array = array;
    params->arraySize = 5;
    params->cs = &cs;
    params->hStart = hStart;
    params->hCantContinue = CreateEvent(NULL, TRUE, FALSE, NULL);
    params->hStop = CreateEvent(NULL, FALSE, FALSE, NULL);
    params->hContinue = CreateEvent(NULL, FALSE, FALSE, NULL);
    params->isActive = true;

    HANDLE hThread = CreateThread(NULL, 0, MarkerThread, params, 0, NULL);
    assert(hThread != NULL);

    SetEvent(hStart);

    WaitForSingleObject(params->hCantContinue, 5000);

    SetEvent(params->hStop);
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(params->hCantContinue);
    CloseHandle(params->hStop);
    CloseHandle(params->hContinue);
    CloseHandle(hStart);
    DeleteCriticalSection(&cs);
    delete params;

    std::cout << "TestThreadLogic passed." << std::endl;
}

int main() {
    TestCriticalSection();
    TestEvents();
    TestThreadLogic();
    std::cout << "All tests passed." << std::endl;
    return 0;
}