#include "hat.h"

#include <iostream>
#include <limits>

int main() {
    GasNetwork network;

    while (true) {
        std::cout << "Gas Network Management System\n";
        std::cout << "1. Add Pipe\n";
        std::cout << "2. Add Station\n";
        std::cout << "3. Show Network\n";
        std::cout << "4. Toggle Pipe Repair Status\n";
        std::cout << "5. Manage Station\n";
        std::cout << "6. Connect Stations\n";
        std::cout << "7. Sort Pipes and Stations\n";
        std::cout << "8. Save Data\n";
        std::cout << "9. Load Data\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Invalid input. Please enter a number between 0 and 9.\n";
            continue;
        }

        switch (choice) {
            case 1:
                network.addPipe();
                break;
            case 2:
                network.addStation();
                break;
            case 3:
                network.showNetwork();
                break;
            case 4:
                network.togglePipeRepair();
                break;
            case 5:
                network.manageStation();
                break;
            case 6:
                network.connectStations();
                break;
            case 7:
                network.sort();
                break;
            case 8:
                network.saveData();
                break;
            case 9:
                network.loadData();
                break;
            case 0:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}