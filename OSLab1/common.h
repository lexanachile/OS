#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <iomanip>

struct employee
{
    int num;
    char name[10];
    double hours;
};

std::istream& operator>>(std::istream& is, employee& e)
{
    is >> e.num >> e.name >> e.hours;
    return is;
}

#endif