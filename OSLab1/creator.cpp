#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

struct EmployeeData {
    int identifier;
    char fullName[10];
    double hoursWorked;
};

bool CreateEmployeeDataFile(const std::string& filename, int recordCount) {
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Cannot create file " << filename << std::endl;
        return false;
    }

    EmployeeData employee;
    for (int i = 0; i < recordCount; i++) {
        std::cout << "\nEnter details for employee #" << (i + 1) << ":\n";

        std::cout << "Employee ID: ";
        while (!(std::cin >> employee.identifier)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a numeric ID: ";
        }

        std::cout << "Employee name (max 9 characters): ";
        std::cin.ignore();
        std::cin.getline(employee.fullName, 10);

        std::cout << "Hours worked: ";
        while (!(std::cin >> employee.hoursWorked) || employee.hoursWorked < 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a positive number: ";
        }

        outputFile.write(reinterpret_cast<char*>(&employee), sizeof(EmployeeData));
    }

    outputFile.close();
    std::cout << "Data file '" << filename << "' created successfully with " << recordCount << " records.\n";
    return true;
}

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