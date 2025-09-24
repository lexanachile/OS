#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

struct EmployeeInfo {
    int id;
    char name[10];
    double hours;
};

bool ExecuteProcess(const std::string& command, const std::string& processName) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::vector<char> cmdLine(command.begin(), command.end());
    cmdLine.push_back('\0');

    if (!CreateProcess(
        NULL,
        cmdLine.data(),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    )) {
        std::cerr << "Error: Failed to start " << processName << " process (Error code: " << GetLastError() << ")\n";
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        std::cerr << "Error: Failed to get exit code for " << processName << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return false;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (exitCode != 0) {
        std::cerr << "Error: " << processName << " process failed with exit code " << exitCode << std::endl;
        return false;
    }

    return true;
}

void DisplayBinaryFileContent(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    EmployeeInfo employee;
    std::cout << "\nContents of binary file '" << filename << "':\n";
    std::cout << "ID\tName\tHours\n";
    std::cout << "----------------\n";

    while (file.read(reinterpret_cast<char*>(&employee), sizeof(EmployeeInfo))) {
        std::cout << employee.id << "\t" << employee.name << "\t" << employee.hours << std::endl;
    }

    file.close();
}

void DisplayTextFileContent(const std::string& filename) {
    UINT originalCP = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        SetConsoleOutputCP(originalCP);
        return;
    }

    char bom[3];
    file.read(bom, 3);
    if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') {
        file.seekg(0);
    }

    std::cout << "\nContents of report file '" << filename << "':\n";
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
    SetConsoleOutputCP(originalCP);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::string dataFilename;
    int recordsCount;

    std::cout << "Enter binary filename: ";
    std::cin >> dataFilename;
    std::cout << "Enter number of records: ";
    std::cin >> recordsCount;

    if (recordsCount <= 0) {
        std::cerr << "Error: Number of records must be a positive integer" << std::endl;
        return 1;
    }

    std::string creatorCommand = "Creator.exe " + dataFilename + " " + std::to_string(recordsCount);
    if (!ExecuteProcess(creatorCommand, "Creator")) {
        return 1;
    }

    DisplayBinaryFileContent(dataFilename);

    std::string reportFilename;
    double hourlyRate;

    std::cout << "\nEnter report filename: ";
    std::cin >> reportFilename;
    std::cout << "Enter hourly rate: ";
    std::cin >> hourlyRate;

    if (hourlyRate <= 0) {
        std::cerr << "Error: Hourly rate must be a positive number" << std::endl;
        return 1;
    }

    std::string reporterCommand = "Reporter.exe " + dataFilename + " " + reportFilename + " " + std::to_string(hourlyRate);
    if (!ExecuteProcess(reporterCommand, "Reporter")) {
        return 1;
    }

    DisplayTextFileContent(reportFilename);

#ifdef TESTING
    return 0;
#endif

    std::cout << "\nAll operations completed successfully. Press Enter to exit...";
    std::cin.ignore();
    std::cin.get();

    return 0;

}