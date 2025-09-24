#include "common.h"

using namespace std;

bool generate_report(const string& bin_filename, const string& report_filename, double hourly_rate)
{
    ifstream file_in(bin_filename, ios::binary);
    if (!file_in.is_open())
    {
        cerr << "Error: Cannot open binary file " << bin_filename << endl;
        return false;
    }

    ofstream report_out(report_filename);
    if (!report_out.is_open())
    {
        cerr << "Error: Cannot create report file " << report_filename << endl;
        file_in.close();
        return false;
    }

    report_out << "Report for file \"" << bin_filename << "\"\n";
    report_out << "Employee number, employee name, hours, salary\n";

    employee emp;
    while (file_in.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
    {
        double salary = emp.hours * hourly_rate;
        report_out << emp.num << ", "
                   << emp.name << ", "
                   << emp.hours << ", "
                   << fixed << setprecision(2) << salary << "\n";
    }

    file_in.close();
    report_out.close();
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cerr << "Usage: reporter.exe <binary_file> <report_file> <hourly_rate>" << endl;
        return 1;
    }

    string bin_file = argv[1];
    string report_file = argv[2];
    double rate = stod(argv[3]);

    if (!generate_report(bin_file, report_file, rate))
    {
        return 1;
    }

    return 0;
}