#include <gtest/gtest.h>
#include <fstream>
#include <windows.h>

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
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return false;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return exitCode == 0;
}

void DisplayBinaryFileContent(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    struct EmployeeInfo {
        int id;
        char name[10];
        double hours;
    } employee;

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
        SetConsoleOutputCP(originalCP);
        return;
    }

    char bom[3];
    file.read(bom, 3);
    if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') {
        file.seekg(0);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    file.close();
    SetConsoleOutputCP(originalCP);
}

TEST(MainTest, ExecuteProcessTest) {
    bool result = ExecuteProcess("cmd /c echo Hello World", "Test Process");
    EXPECT_TRUE(result);
}

TEST(MainTest, DisplayBinaryFileContentTest) {
    struct EmployeeInfo {
        int id;
        char name[10];
        double hours;
    } testEmployees[] = {
        {101, "John", 8.5},
        {102, "Jane", 7.0}
    };

    std::string testFilename = "test_display.bin";
    std::ofstream testFile(testFilename, std::ios::binary);
    for (const auto& emp : testEmployees) {
        testFile.write(reinterpret_cast<const char*>(&emp), sizeof(EmployeeInfo));
    }
    testFile.close();

    testing::internal::CaptureStdout();
    DisplayBinaryFileContent(testFilename);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("101"), std::string::npos);
    EXPECT_NE(output.find("John"), std::string::npos);
    EXPECT_NE(output.find("8.5"), std::string::npos);

    remove(testFilename.c_str());
}

TEST(MainTest, DisplayTextFileContentTest) {
    std::string testFilename = "test_display.txt";
    std::ofstream testFile(testFilename);
    testFile << "Line 1\nLine 2\nLine 3";
    testFile.close();

    testing::internal::CaptureStdout();
    DisplayTextFileContent(testFilename);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Line 1"), std::string::npos);
    EXPECT_NE(output.find("Line 2"), std::string::npos);
    EXPECT_NE(output.find("Line 3"), std::string::npos);

    remove(testFilename.c_str());
}