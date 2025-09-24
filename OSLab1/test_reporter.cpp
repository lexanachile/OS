#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "common.h"

using namespace std;

TEST(ReporterTest, GenerateReportSuccess)
{
    string bin_file = "test_bin.bin";
    string report_file = "test_report.txt";

    employee test_employees[] = {
        {1, "Ivanov", 10.0},
        {2, "Petrov", 20.0}
    };

    ofstream bin_out(bin_file, ios::binary);
    for (const auto& emp : test_employees)
    {
        bin_out.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }
    bin_out.close();

    ifstream file_in(bin_file, ios::binary);
    ofstream report_out(report_file);

    report_out << "Report for file \"" << bin_file << "\"\n";
    report_out << "Employee number, employee name, hours, salary\n";

    employee emp;
    while (file_in.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
    {
        double salary = emp.hours * 150.0;
        report_out << emp.num << ", "
                   << emp.name << ", "
                   << emp.hours << ", "
                   << fixed << setprecision(2) << salary << "\n";
    }

    file_in.close();
    report_out.close();

    ifstream report_in(report_file);
    ASSERT_TRUE(report_in.is_open());

    stringstream buffer;
    buffer << report_in.rdbuf();
    string content = buffer.str();

    EXPECT_NE(content.find("Report for file \"test_bin.bin\""), string::npos);
    EXPECT_NE(content.find("Employee number, employee name, hours, salary"), string::npos);
    EXPECT_NE(content.find("1, Ivanov, 10.00, 1500.00"), string::npos);
    EXPECT_NE(content.find("2, Petrov, 20.00, 3000.00"), string::npos);

    report_in.close();
    remove(bin_file.c_str());
    remove(report_file.c_str());
}