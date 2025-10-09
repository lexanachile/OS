#include "ArrayThreads.h"
#include <iostream>
#include <boost/thread.hpp>

class ArrayProcessor {
public:
    static bool validateInput(int size) {
        return size > 0;
    }
    static void handleThreadError() {
        std::cout << "Thread creation error\n";
    }
};

int main() {
    ArrayData data = {0};
    std::cout << "Enter array size: ";
    std::cin >> data.numOfElements;
    if (!ArrayProcessor::validateInput(data.numOfElements)) {
        std::cout << "Error: array size must be positive\n";
        return 1;
    }
    data.TaskArray = new int[data.numOfElements];
    std::cout << "Enter " << data.numOfElements << " elements: ";
    for (int i = 0; i < data.numOfElements; ++i) {
        std::cin >> data.TaskArray[i];
    }
    try {
        boost::thread thread_min_max(min_max, &data);
        boost::thread thread_average(average, &data);
        thread_min_max.join();
        thread_average.join();
    } catch (const boost::thread_exception&) {
        ArrayProcessor::handleThreadError();
        delete[] data.TaskArray;
        return 1;
    }
    replaceMinMaxWithAverage(data);
    std::cout << "Result array: ";
    for (int i = 0; i < data.numOfElements; ++i) {
        std::cout << data.TaskArray[i] << ' ';
    }
    std::cout << std::endl;
    delete[] data.TaskArray;
    return 0;
}
