#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

struct EmployeeData {
    int identifier;
    char fullName[10];
    double hoursWorked;
};

bool CreateEmployeeDataFile(const std::string& filename, int recordCount, std::istream& inputStream = std::cin, std::ostream& outputStream = std::cout) {
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

#ifndef TESTING
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: Creator <filename> <record_count>" << std::endl;
        return 1;
    }

    int recordCount;
    try {
        recordCount = std::stoi(argv[2]);
        if (recordCount <= 0) {
            std::cerr << "Error: Record count must be a positive integer" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid record count - " << e.what() << std::endl;
        return 1;
    }

    if (!CreateEmployeeDataFile(argv[1], recordCount)) {
        return 1;
    }

    return 0;
}
#endif