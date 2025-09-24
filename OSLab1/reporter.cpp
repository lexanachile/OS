#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>

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

#ifndef TESTING
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Reporter <input_file> <output_file> <hourly_rate>" << std::endl;
        return 1;
    }

    double hourlyRate;
    try {
        hourlyRate = std::stod(argv[3]);
        if (hourlyRate <= 0) {
            std::cerr << "Error: Hourly rate must be a positive number" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid hourly rate - " << e.what() << std::endl;
        return 1;
    }

    if (!GenerateSalaryReport(argv[1], argv[2], hourlyRate)) {
        return 1;
    }

    std::cout << "Report '" << argv[2] << "' generated successfully.\n";
    return 0;
}
#endif