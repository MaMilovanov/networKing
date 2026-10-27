#include "hat.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace {

    int readInt(const std::string& vvod) {
        int value;
        while (true) {
            std::cout << vvod;
            if (std::cin >> value) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cout << "invalid input.BEZ DROBEI!!!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    double readPositiveDoubles(const std::string& vvod) {
        double value;
        while (true) {
            std::cout << vvod;
            if (std::cin >> value && value > 0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cout << "Invalid input. Please enter positive number\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string readString(const std::string& vvod) {
        std::string value;
        while (true) {
            std::cout << vvod;
            if (std::getline(std::cin, value) && !value.empty()) {
                return value;
            }
            std::cout << "Invalid input. Please enter sth\n";
        }    
    }

    int readDiameter() {
        while (true) {
            int diameter = readInt("Availible diameters: 500, 700, 1000, 1400: ");
            if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) {
                return diameter;
            }
            std::cout << "your name Invalid ?. Please enter availible diameter\n";
        }
    }

    bool ask(const std::string& question) {
        while (true) {
            int answer = readInt(question + " (y for Yes, n for No): ");
            if (answer == 'y' || answer == 'Y') {
                return answer=='y' || answer=='Y';
            }
            std::cout << "Please enter 'y' or 'n'\n";
        }
    }
}