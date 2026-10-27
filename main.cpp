#include "hat.h"

#include <iostream>
#include <limits>

namespace {
    int readMenuChoice() {
        int choice;
        while (true) {
            std::cout << "Enter your choce: ";
            if (std::cin >> choice) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return choice;
            }
            std::cout << "Invalid input. Please enter menu choice\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int main() {
    GasNetwork network;

    while (true) {
        std::cout << "\n========== GAS NETWORK MANAGEMENT SYSTEM ==========\n"
                  << "1. Add Pipe\n"
                  << "2. Add Station\n"
                  << "3. Show Network\n"
                  << "4. Toggle Pipe Repair Status\n"
                  << "5. Manage Station Workshops\n"
                  << "6. Connect Stations\n"
                  << "7. Topological Sort\n"
                  << "8. Save Data\n"
                  << "9. Load Data\n"
                  << "10. Delete Pipe\n"
                  << "11. Edit Station\n"
                  << "12. Delete Station\n"
                  << "13. Search Pipes\n"
                  << "14. Search Stations\n"
                  << "15. Batch Edit/Delete Pipes\n"
                  << "0. Exit\n";

        int choice = readMenuChoice();
        network.logAction("MENU", "choice=" + std::to_string(choice));

        switch (choice) {
            case 1: network.addPipe(); break;
            case 2: network.addStation(); break;
            case 3: network.showNetwork(); break;
            case 4: network.togglePipeRepair(); break;
            case 5: network.manageStation(); break;
            case 6: network.connectStations(); break;
            case 7: network.sort(); break;
            case 8: network.saveData(); break;
            case 9: network.loadData(); break;
            case 10: network.deletePipe(); break;
            case 11: network.editStation(); break;
            case 12: network.deleteStation(); break;
            case 13: network.searchPipes(); break;
            case 14: network.searchStations(); break;
            case 15: network.batchEditPipes(); break;
            case 0:
                network.logAction("EXIT", "User exited the program");
                std::cout << "Program finished.\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                network.logAction("INVALID_MENU_CHOICE", "choice=" + std::to_string(choice));
        }
    }
}
