#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

struct EmployeeRecord {
    int employeeId;
    char employeeName[10];
    double workedHours;
};

bool GenerateSalaryReport(const std::string& inputFile, const std::string& outputFile, double hourlyRate) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Error: Cannot open input file " << inputFile << std::endl;
        return false;
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot create output file " << outputFile << std::endl;
        inFile.close();
        return false;
    }

    const unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    outFile.write(reinterpret_cast<const char*>(bom), sizeof(bom));

    outFile << "File report \"" << inputFile << "\"\n";
    outFile << "Employee's number\tEmployee's name\tTime\tSalary\n";

    EmployeeRecord employee;
    while (inFile.read(reinterpret_cast<char*>(&employee), sizeof(EmployeeRecord))) {
        double salary = employee.workedHours * hourlyRate;
        outFile << employee.employeeId << "\t\t\t"
                << employee.employeeName << "\t\t"
                << employee.workedHours << "\t"
                << std::fixed << std::setprecision(2) << salary << "\n";
    }

    inFile.close();
    outFile.close();
    return true;
}

TEST(ReporterTest, GenerateSalaryReportTest) {
    std::string inputFilename = "test_input.bin";
    std::string outputFilename = "test_report.txt";

    EmployeeRecord testEmployees[] = {
        {101, "John", 8.5},
        {102, "Jane", 7.0}
    };

    std::ofstream testFile(inputFilename, std::ios::binary);
    for (const auto& emp : testEmployees) {
        testFile.write(reinterpret_cast<const char*>(&emp), sizeof(EmployeeRecord));
    }
    testFile.close();

    double hourlyRate = 10.0;

    bool result = GenerateSalaryReport(inputFilename, outputFilename, hourlyRate);

    EXPECT_TRUE(result);

    std::ifstream reportFile(outputFilename);
    std::string reportContent;
    std::string line;
    while (std::getline(reportFile, line)) {
        reportContent += line + "\n";
    }
    reportFile.close();

    EXPECT_NE(reportContent.find("101"), std::string::npos);
    EXPECT_NE(reportContent.find("John"), std::string::npos);
    EXPECT_NE(reportContent.find("8.5"), std::string::npos);
    EXPECT_NE(reportContent.find("85.00"), std::string::npos);

    remove(inputFilename.c_str());
    remove(outputFilename.c_str());
}

TEST(ReporterTest, EmptyInputTest) {
    std::string inputFilename = "empty_test.bin";
    std::string outputFilename = "empty_report.txt";

    std::ofstream testFile(inputFilename, std::ios::binary);
    testFile.close();

    double hourlyRate = 10.0;

    bool result = GenerateSalaryReport(inputFilename, outputFilename, hourlyRate);

    EXPECT_TRUE(result);

    std::ifstream reportFile(outputFilename);
    std::string reportContent;
    std::string line;
    while (std::getline(reportFile, line)) {
        reportContent += line + "\n";
    }
    reportFile.close();

    EXPECT_NE(reportContent.find("File report"), std::string::npos);
    EXPECT_NE(reportContent.find("Employee's number"), std::string::npos);

    remove(inputFilename.c_str());
    remove(outputFilename.c_str());
}