#include "common.h"

using namespace std;

void print_binary_file(const string& filename)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Error: Cannot open binary file to display." << endl;
        return;
    }

    employee emp;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee)))
    {
        cout << emp.num << " " << emp.name << " " << emp.hours << endl;
    }
    file.close();
}

int main()
{
    string binary_file_name;
    int entries_amount;

    cout << "Enter binary file name: ";
    cin >> binary_file_name;
    cout << "Enter number of employee records to create: ";
    cin >> entries_amount;

    string creator_cmd = "creator.exe " + binary_file_name + " " + to_string(entries_amount);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcessA(NULL, &creator_cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        cerr << "Couldn't create Creator process" << endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    cout << "\nBinary file content:" << endl;
    print_binary_file(binary_file_name);

    string report_file_name;
    double hourly_rate;

    cout << "\nEnter report file name: ";
    cin >> report_file_name;
    cout << "Enter hourly rate for salary calculation: ";
    cin >> hourly_rate;

    string reporter_cmd = "reporter.exe " + binary_file_name + " " + report_file_name + " " + to_string(hourly_rate);

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcessA(NULL, &reporter_cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        cerr << "Couldn't create Reporter process" << endl;
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    cout << "\nReport content:" << endl;
    ifstream report_file(report_file_name);
    if (report_file.is_open())
    {
        string line;
        while (getline(report_file, line))
        {
            cout << line << endl;
        }
        report_file.close();
    }

    return 0;
}