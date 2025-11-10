#include <iostream>
#include <windows.h>
#include <string>
#include <cstring>
#include <vector>
#include "../src/common.h"
#include "../src/queue_ops.h"

#define ASSERT(x) if(!(x)) return false;

const std::string TEST_FILE = "test.bin";

HANDLE openFileHandle() {
    return CreateFileA(TEST_FILE.c_str(), GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

bool test_write_read() {
    int records = 5;
    initQueueFile(TEST_FILE, records);
    HANDLE hFile = openFileHandle();

    Message m1, m2;
    strcpy_s(m1.text, "Msg1");
    strcpy_s(m2.text, "Msg2");

    writeMessageToQueue(hFile, m1, records);
    writeMessageToQueue(hFile, m2, records);

    Message out1 = readMessageFromQueue(hFile, records);
    Message out2 = readMessageFromQueue(hFile, records);

    ASSERT(strcmp(out1.text, "Msg1") == 0);
    ASSERT(strcmp(out2.text, "Msg2") == 0);

    CloseHandle(hFile);
    return true;
}

bool test_circular() {
    int records = 2;
    initQueueFile(TEST_FILE, records);
    HANDLE hFile = openFileHandle();

    Message m1, m2, m3;
    strcpy_s(m1.text, "A");
    strcpy_s(m2.text, "B");
    strcpy_s(m3.text, "C");

    writeMessageToQueue(hFile, m1, records);
    writeMessageToQueue(hFile, m2, records);

    readMessageFromQueue(hFile, records);

    writeMessageToQueue(hFile, m3, records);

    Message out = readMessageFromQueue(hFile, records);
    ASSERT(strcmp(out.text, "B") == 0);

    out = readMessageFromQueue(hFile, records);
    ASSERT(strcmp(out.text, "C") == 0);

    CloseHandle(hFile);
    return true;
}

int main() {
    std::vector<bool(*)()> tests = { test_write_read, test_circular };
    int passed = 0;

    for (auto test : tests) {
        DeleteFileA(TEST_FILE.c_str());
        if (test()) passed++;
    }

    DeleteFileA(TEST_FILE.c_str());

    std::cout << "Passed: " << passed << "/" << tests.size() << std::endl;
    return (passed == tests.size()) ? 0 : 1;
}