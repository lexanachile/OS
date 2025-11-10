#include "queue_ops.h"
#include <iostream>

bool initQueueFile(const std::string& filename, int records_count) {
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) return false;

    QueueHeader header = { 0, 0 };
    DWORD bytesWritten;
    WriteFile(hFile, &header, sizeof(QueueHeader), &bytesWritten, NULL);
    
    SetFilePointer(hFile, sizeof(QueueHeader) + records_count * sizeof(Message), NULL, FILE_BEGIN);
    SetEndOfFile(hFile);

    CloseHandle(hFile);
    return true;
}

QueueHeader readHeader(HANDLE hFile) {
    QueueHeader header;
    DWORD bytesRead;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    ReadFile(hFile, &header, sizeof(QueueHeader), &bytesRead, NULL);
    return header;
}

void writeHeader(HANDLE hFile, const QueueHeader& header) {
    DWORD bytesWritten;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    WriteFile(hFile, &header, sizeof(QueueHeader), &bytesWritten, NULL);
}

Message readMessageFromQueue(HANDLE hFile, int records_count) {
    QueueHeader header = readHeader(hFile);
    Message msg;
    DWORD bytesRead;

    SetFilePointer(hFile, sizeof(QueueHeader) + header.read_pos * sizeof(Message), NULL, FILE_BEGIN);
    ReadFile(hFile, &msg, sizeof(Message), &bytesRead, NULL);

    header.read_pos = (header.read_pos + 1) % records_count;
    writeHeader(hFile, header);

    return msg;
}

void writeMessageToQueue(HANDLE hFile, const Message& msg, int records_count) {
    QueueHeader header = readHeader(hFile);
    DWORD bytesWritten;

    SetFilePointer(hFile, sizeof(QueueHeader) + header.write_pos * sizeof(Message), NULL, FILE_BEGIN);
    WriteFile(hFile, &msg, sizeof(Message), &bytesWritten, NULL);

    header.write_pos = (header.write_pos + 1) % records_count;
    writeHeader(hFile, header);
}