#include "common.h"

using namespace std;

bool write_employees_to_file(const string& filename, int amount)
{
    ofstream file_out(filename, ios::binary);
    if (!file_out.is_open())
    {
        cerr << "Error: Cannot create file " << filename << endl;
        return false;
    }

    cout << "Enter employee data (ID Name Hours) for " << amount << " employees:" << endl;
    for (int i = 0; i < amount; ++i)
    {
        employee emp;
        cout << "Employee " << (i + 1) << ": ";
        cin >> emp;
        file_out.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }

    file_out.close();
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: creator.exe <filename> <record_count>" << endl;
        return 1;
    }

    string filename = argv[1];
    int record_count = stoi(argv[2]);

    if (!write_employees_to_file(filename, record_count))
    {
        return 1;
    }

    return 0;
}