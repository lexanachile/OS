#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "common.h"

using namespace std;

TEST(CreatorTest, CreateFileWithEmployees)
{
    string test_filename = "test_employees.bin";

    stringstream input;
    input << "101 Ivan 40.5\n102 Petr 35.0\n";
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());

    ofstream file_out(test_filename, ios::binary);
    for (int i = 0; i < 2; ++i)
    {
        employee emp;
        cin >> emp;
        file_out.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }
    file_out.close();

    cin.rdbuf(orig_cin);

    ifstream file(test_filename, ios::binary);
    ASSERT_TRUE(file.is_open());

    employee emp1, emp2;
    file.read(reinterpret_cast<char*>(&emp1), sizeof(employee));
    file.read(reinterpret_cast<char*>(&emp2), sizeof(employee));

    EXPECT_EQ(emp1.num, 101);
    EXPECT_STREQ(emp1.name, "Ivan");
    EXPECT_DOUBLE_EQ(emp1.hours, 40.5);

    EXPECT_EQ(emp2.num, 102);
    EXPECT_STREQ(emp2.name, "Petr");
    EXPECT_DOUBLE_EQ(emp2.hours, 35.0);

    file.close();
    remove(test_filename.c_str());
}

TEST(CreatorTest, InvalidFilename)
{
    ofstream file_out("/invalid/path/test.bin", ios::binary);
    EXPECT_FALSE(file_out.is_open());
}