#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

struct EmployeeData {
    int identifier;
    char fullName[10];
    double hoursWorked;
};

bool CreateEmployeeDataFile(const std::string& filename, int recordCount, std::istream& inputStream, std::ostream& outputStream) {
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open()) {
        outputStream << "Error: Cannot create file " << filename << std::endl;
        return false;
    }

    EmployeeData employee;
    for (int i = 0; i < recordCount; i++) {
        outputStream << "\nEnter details for employee #" << (i + 1) << ":\n";

        outputStream << "Employee ID: ";
        while (!(inputStream >> employee.identifier)) {
            inputStream.clear();
            inputStream.ignore(10000, '\n');
            outputStream << "Invalid input. Please enter a numeric ID: ";
        }

        outputStream << "Employee name (max 9 characters): ";
        inputStream.ignore();
        inputStream.getline(employee.fullName, 10);

        outputStream << "Hours worked: ";
        while (!(inputStream >> employee.hoursWorked) || employee.hoursWorked < 0) {
            inputStream.clear();
            inputStream.ignore(10000, '\n');
            outputStream << "Invalid input. Please enter a positive number: ";
        }

        outputFile.write(reinterpret_cast<char*>(&employee), sizeof(EmployeeData));
    }

    outputFile.close();
    outputStream << "Data file '" << filename << "' created successfully with " << recordCount << " records.\n";
    return true;
}

TEST(CreatorTest, CreateEmployeeDataFileTest) {
    std::string testFilename = "test_data.bin";
    int recordCount = 2;

    std::stringstream testInput;
    testInput << "101\nJohn\n8.5\n102\nJane\n7.0\n";

    std::stringstream testOutput;

    bool result = CreateEmployeeDataFile(testFilename, recordCount, testInput, testOutput);

    EXPECT_TRUE(result);

    std::ifstream file(testFilename, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    EmployeeData emp;
    file.read(reinterpret_cast<char*>(&emp), sizeof(EmployeeData));
    EXPECT_EQ(emp.identifier, 101);
    EXPECT_STREQ(emp.fullName, "John");
    EXPECT_DOUBLE_EQ(emp.hoursWorked, 8.5);

    file.read(reinterpret_cast<char*>(&emp), sizeof(EmployeeData));
    EXPECT_EQ(emp.identifier, 102);
    EXPECT_STREQ(emp.fullName, "Jane");
    EXPECT_DOUBLE_EQ(emp.hoursWorked, 7.0);

    file.close();
    remove(testFilename.c_str());
}

TEST(CreatorTest, InvalidInputTest) {
    std::string testFilename = "test_invalid.bin";
    int recordCount = 1;

    std::stringstream testInput;
    testInput << "invalid\n101\nJohn\n8.5\n";

    std::stringstream testOutput;

    bool result = CreateEmployeeDataFile(testFilename, recordCount, testInput, testOutput);

    EXPECT_TRUE(result);

    std::ifstream file(testFilename, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    EmployeeData emp;
    file.read(reinterpret_cast<char*>(&emp), sizeof(EmployeeData));
    EXPECT_EQ(emp.identifier, 101);
    EXPECT_STREQ(emp.fullName, "John");
    EXPECT_DOUBLE_EQ(emp.hoursWorked, 8.5);

    file.close();
    remove(testFilename.c_str());
}