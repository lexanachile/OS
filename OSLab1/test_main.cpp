#include <gtest/gtest.h>
#include <fstream>
#include "common.h"

using namespace std;

void print_binary_file(const string& filename)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        throw runtime_error("Cannot open file");
    }

    employee emp;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
    {
        cout << emp.num << " " << emp.name << " " << emp.hours << endl;
    }
    file.close();
}

TEST(MainTest, PrintBinaryFile)
{
    string test_file = "test_print.bin";

    employee test_employees[] = {
        {101, "John", 8.5},
        {102, "Jane", 7.0}
    };

    ofstream file_out(test_file, ios::binary);
    for (const auto& emp : test_employees)
    {
        file_out.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }
    file_out.close();

    testing::internal::CaptureStdout();
    print_binary_file(test_file);
    string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("101"), string::npos);
    EXPECT_NE(output.find("John"), string::npos);
    EXPECT_NE(output.find("8.5"), string::npos);
    EXPECT_NE(output.find("102"), string::npos);
    EXPECT_NE(output.find("Jane"), string::npos);
    EXPECT_NE(output.find("7"), string::npos);

    remove(test_file.c_str());
}

TEST(MainTest, PrintNonExistentFile)
{
    EXPECT_THROW(print_binary_file("nonexistent.bin"), runtime_error);
}

TEST(CommonTest, EmployeeInputOperator)
{
    istringstream input("123 Ivan 45.5");
    employee emp;

    input >> emp;

    EXPECT_EQ(emp.num, 123);
    EXPECT_STREQ(emp.name, "Ivan");
    EXPECT_DOUBLE_EQ(emp.hours, 45.5);
}