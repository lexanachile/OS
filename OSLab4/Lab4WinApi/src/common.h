#pragma once
#include <windows.h>

const char MUTEX_NAME[] = "Global\\Lab4Mutex";
const char SEM_EMPTY_NAME[] = "Global\\Lab4SemEmpty";
const char SEM_FULL_NAME[] = "Global\\Lab4SemFull";
const char SEM_READY_NAME[] = "Global\\Lab4SemReady";

struct Message {
    char text[20];
};

struct QueueHeader {
    int read_pos;
    int write_pos;
};