#include "hat.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
            if (std::getline(std::cin >> std::ws, value) && !value.empty()) {
                return value;
            }
            std::cout << "Invalid input. Please enter a non-empty value.\n";
            if (!std::cin) {
                std::cin.clear();
            }
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
            int answer = readInt(question + " (1 - Yes, 0 - No): ");
            if (answer == 0 || answer == 1) return answer == 1;
            std::cout << "Please enter 1 or 0.\n";
        }
    }

    std::string lowerCopy(std::string value) {
        for (char& ch : value) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
        return value;
    }
}

void GasNetwork::logAction(const std::string& action, const std::string& details) const {
    std::ofstream logFile("user_actions.log", std::ios::app);
    if (!logFile) {
        std::cerr << "Warning: cannot write to user_actions.log\n";
        return;
    }

    const auto now = std::chrono::system_clock::now();
    const std::time_t timeValue = std::chrono::system_clock::to_time_t(now);
    const std::tm* localTime = std::localtime(&timeValue);
    if (localTime) {
        logFile << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    } else {
        logFile << "unknown-time";
    }
    logFile << " | " << action << " | " << details << '\n';
}

int GasNetwork::nextPipeId() const {
    int maxId = 0;
    for (const Pipe& pipe : pipes) {
        maxId = std::max(maxId, pipe.id);
    }
    return maxId + 1;
}

int GasNetwork::nextStationId() const {
    int maxId = 0;
    for (const Station& station : stations) {
        maxId = std::max(maxId, station.id);
    }
    return maxId + 1;
}

Pipe* GasNetwork::findPipe(int id) {
    for (Pipe& pipe : pipes) {
        if (pipe.id == id) return &pipe;
    }
    return nullptr;
}

const Pipe* GasNetwork::findPipe(int id) const {
    for (const Pipe& pipe : pipes) {
        if (pipe.id == id) return &pipe;
    }
    return nullptr;
}

Station* GasNetwork::findStation(int id) {
    for (Station& station : stations) {
        if (station.id == id) return &station;
    }
    return nullptr;
}

const Station* GasNetwork::findStation(int id) const {
    for (const Station& station : stations) {
        if (station.id == id) return &station;
    }
    return nullptr;
}

void GasNetwork::addPipe() {
    Pipe pipe;
    pipe.id = nextPipeId();
    pipe.name = readString("Enter pipe name: ");
    pipe.length = readPositiveDoubles("Enter pipe length in kilometers: ");
    pipe.diameter = readDiameter();
    pipe.inRepair = false;
    pipe.isConnected = false;

    pipes.push_back(pipe);
    std::cout << "Pipe created. Pipe ID: " << pipe.id << "\n";
    logAction("ADD_PIPE", "ID=" + std::to_string(pipe.id) + ", name=" + pipe.name);
}

void GasNetwork::addStation() {
    Station station;
    station.id = nextStationId();
    station.name = readString("Enter station name: ");

    do {
        station.totalNasos = readInt("Enter total number of workshops: ");
        if (station.totalNasos <= 0) {
            std::cout << "The number of workshops must be greater than zero\n";
        }
    } while (station.totalNasos <= 0);

    do {
        station.activeNasos = readInt("Enter number of active workshops: ");
        if (station.activeNasos < 0 || station.activeNasos > station.totalNasos) {
            std::cout << "The number of active workshops must be between 0 and "
                      << station.totalNasos << "\n";
        }
    } while (station.activeNasos < 0 || station.activeNasos > station.totalNasos);

    station.totalRepairWorksNasos = 0;
    stations.push_back(station);
    std::cout << "Station created. Station ID: " << station.id << "\n";
    logAction("ADD_STATION", "ID=" + std::to_string(station.id) + ", name=" + station.name);
}

void GasNetwork::showNetwork() const {
    std::cout << "\n<<=(}{)=<>=(}{)=| PIPES |=(}{)=<>=(}{)=>>\n";
    if (pipes.empty()) {
        std::cout << "No pipes now.\n";
    } else {
        for (const Pipe& pipe : pipes) {
            std::cout << "ID: " << pipe.id
                      << " | Name: " << pipe.name
                      << " | Length: " << pipe.length << " km"
                      << " | Diameter: " << pipe.diameter << " mm"
                      << " | Repair: " << (pipe.inRepair ? "Yes" : "No")
                      << " | Connected: " << (pipe.isConnected ? "Yes" : "No")
                      << "\n";
        }
    }

    std::cout << "\n<<=(}{)=<>=(}{)=| STATIONS |=(}{)=<>=(}{)=>>\n";
    if (stations.empty()) {
        std::cout << "No stations now.\n";
    } else {
        for (const Station& station : stations) {
            std::cout << "ID: " << station.id
                      << " | Name: " << station.name
                      << " | Active Workshops: " << station.activeNasos
                      << "/" << station.totalNasos
                      << " | Workshops in Repair: " << station.totalRepairWorksNasos
                      << "\n";
        }
    }

    std::cout << "\n<<=(}{)=<>=(}{)=| CONNECTIONS |=(}{)=<>=(}{)=>>\n";
    if (connections.empty()) {
        std::cout << "No connections now\n";
    } else {
        for (const Connection& connection : connections) {
            const Station* from = findStation(connection.fromStation);
            const Station* to = findStation(connection.toStation);
            const Pipe* pipe = findPipe(connection.pipeId);
            std::cout << "Pipe ID " << connection.pipeId << ": Station "
                      << connection.fromStation;
            if (from) std::cout << " (" << from->name << ")";
            std::cout << " -> Station " << connection.toStation;
            if (to) std::cout << " (" << to->name << ")";
            if (pipe) std::cout << ", diameter " << pipe->diameter << " mm";
            std::cout << "\n";
        }
    }
}

void GasNetwork::togglePipeRepair() {
    if (pipes.empty()) {
        std::cout << "First, create a pipe\n";
        logAction("TOGGLE_PIPE_REPAIR_FAILED", "No pipes exist");
        return;
    }

    int id = readInt("Enter pipe ID: ");
    Pipe* pipe = findPipe(id);
    if (!pipe) {
        std::cout << "Pipe with such ID not found\n";
        logAction("TOGGLE_PIPE_REPAIR_FAILED", "Pipe ID=" + std::to_string(id) + " not found");
        return;
    }

    pipe->inRepair = !pipe->inRepair;
    std::cout << "Pipe state changed: "
              << (pipe->inRepair ? "in repair" : "not in repair") << ".\n";
    logAction("TOGGLE_PIPE_REPAIR", "ID=" + std::to_string(id) + ", inRepair=" + (pipe->inRepair ? "true" : "false"));
}

void GasNetwork::manageStation() {
    if (stations.empty()) {
        std::cout << "First, create a station\n";
        logAction("MANAGE_STATION_FAILED", "No stations exist");
        return;
    }

    int id = readInt("Enter station ID: ");
    Station* station = findStation(id);
    if (!station) {
        std::cout << "Station with such ID not found\n";
        logAction("MANAGE_STATION_FAILED", "ID=" + std::to_string(id) + " not found");
        return;
    }

    std::cout << "1. Start workshops\n"
              << "2. Stop workshops\n"
              << "3. Send free workshops to repair\n";
    int choice = readInt("Your choice: ");

    if (choice == 1) {
        int freeShops = station->totalNasos - station->activeNasos
                      - station->totalRepairWorksNasos;
        if (freeShops <= 0) {
            std::cout << "No free workshops available\n";
            return;
        }
        int count = readInt("How many workshops to start? ");
        if (count < 0 || count > freeShops) {
            std::cout << "Invalid quantity/ Free workshops: "
                      << freeShops << ".\n";
            return;
        }
        station->activeNasos += count;
        std::cout << "Workshops started.\n";
    } else if (choice == 2) {
        if (station->activeNasos == 0) {
            std::cout << "No active workshops.\n";
            return;
        }
        int count = readInt("How many workshops to stop? ");
        if (count < 0 || count > station->activeNasos) {
            std::cout << "Invalid quantity\n";
            return;
        }
        station->activeNasos -= count;
        std::cout << "Workshops stopped.\n";
    } else if (choice == 3) {
        int freeShops = station->totalNasos - station->activeNasos
                      - station->totalRepairWorksNasos;
        if (freeShops <= 0) {
            std::cout << "No free workshops available for repair\n";
            return;
        }
        int count = readInt("How many workshops to send to repair? ");
        if (count < 0 || count > freeShops) {
            std::cout << "Invalid quantity\n";
            return;
        }
        station->totalRepairWorksNasos += count;
        std::cout << "Workshops sent to repair.\n";
    } else {
        std::cout << "Unknown station management option.\n";
        logAction("MANAGE_STATION_FAILED", "ID=" + std::to_string(id) + ", invalid option=" + std::to_string(choice));
        return;
    }
    logAction("MANAGE_STATION", "ID=" + std::to_string(id) + ", option=" + std::to_string(choice));
}

void GasNetwork::connectStations() {
    if (stations.size() < 2) {
        std::cout << "For connection, need to create at least two stations\n";
        logAction("CONNECT_STATIONS_FAILED", "Fewer than two stations");
        return;
    }

    std::cout << "\nAvailable Stations:\n";
    for (const Station& station : stations) {
        std::cout << "ID " << station.id << " - " << station.name << "\n";
    }

    int fromId = readInt("Enter ID of the input station: ");
    int toId = readInt("Enter ID of the output station: ");

    if (!findStation(fromId) || !findStation(toId)) {
        std::cout << "One of the stations not found\n";
        logAction("CONNECT_STATIONS_FAILED", "unknown endpoint: " + std::to_string(fromId) + " -> " + std::to_string(toId));
        return;
    }
    if (fromId == toId) {
        std::cout << "Cannot connect a station to itself\n";
        logAction("CONNECT_STATIONS_FAILED", "same endpoint ID=" + std::to_string(fromId));
        return;
    }

    int diameter = readDiameter();

    for (const Connection& connection : connections) {
        if (connection.fromStation == fromId &&
            connection.toStation == toId) {
            std::cout << "These stations are already connected\n";
            logAction("CONNECT_STATIONS_FAILED", "duplicate connection " + std::to_string(fromId) + " -> " + std::to_string(toId));
            return;
        }
    }

    Pipe* availablePipe = nullptr;
    for (Pipe& pipe : pipes) {
        if (pipe.diameter == diameter && !pipe.isConnected && !pipe.inRepair) {
            availablePipe = &pipe;
            break;
        }
    }

    if (!availablePipe) {
        std::cout << "No free pipe of diameter " << diameter << " mm found\n";
        if (!ask("Create a new pipe?")) {
            std::cout << "Connection cancelled\n";
            logAction("CONNECT_STATIONS_CANCELLED", std::to_string(fromId) + " -> " + std::to_string(toId));
            return;
        }

        Pipe pipe;
        pipe.id = nextPipeId();
        std::cout << "Creating pipe for connection\n";
        pipe.name = readString("Enter pipe designation: ");
        pipe.length = readPositiveDoubles("Enter pipe length in kilometers: ");
        pipe.diameter = diameter;
        pipe.inRepair = false;
        pipe.isConnected = false;
        pipes.push_back(pipe);
        availablePipe = &pipes.back();
    }

    availablePipe->isConnected = true;
    connections.push_back({availablePipe->id, fromId, toId});

    std::cout << "Station " << fromId << " is connected to station " << toId
              << " with pipe ID " << availablePipe->id
              << " (diameter " << diameter << " mm)\n";
    logAction("CONNECT_STATIONS", "from=" + std::to_string(fromId) + ", to=" +
              std::to_string(toId) + ", pipeID=" + std::to_string(availablePipe->id));
}

void GasNetwork::sort() const {
    if (stations.empty()) {
        std::cout << "No stations available for sorting\n";
        logAction("TOPOLOGICAL_SORT_FAILED", "No stations exist");
        return;
    }

    std::unordered_map<int, std::vector<int>> adjacency;
    std::unordered_map<int, int> inDegree;

    for (const Station& station : stations) {
        adjacency[station.id];
        inDegree[station.id] = 0;
    }

    for (const Connection& connection : connections) {
        if (inDegree.find(connection.fromStation) == inDegree.end() ||
            inDegree.find(connection.toStation) == inDegree.end()) {
            std::cout << "Connection refers to non-exist station"
                         "Sorting is not possible\n";
            return;
        }
        adjacency[connection.fromStation].push_back(connection.toStation);
        ++inDegree[connection.toStation];
    }

    std::queue<int> ready;
    for (const Station& station : stations) {
        if (inDegree[station.id] == 0) {
            ready.push(station.id);
        }
    }

    std::vector<int> order;
    while (!ready.empty()) {
        int current = ready.front();
        ready.pop();
        order.push_back(current);

        for (int next : adjacency[current]) {
            --inDegree[next];
            if (inDegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    if (order.size() != stations.size()) {
        std::cout << "Sorting is not possible: a cycle has been detected in the network\n";
        logAction("TOPOLOGICAL_SORT_FAILED", "Cycle detected");
        return;
    }

    std::cout << "Topological order of stations:\n";
    for (std::size_t i = 0; i < order.size(); ++i) {
        const Station* station = findStation(order[i]);
        if (i > 0) std::cout << " -> ";
        std::cout << "Station " << order[i];
        if (station) std::cout << " (" << station->name << ")";
    }
    std::cout << "\n";
    logAction("TOPOLOGICAL_SORT", "success, stationCount=" + std::to_string(order.size()));
}


// Removes a pipe and any connection that used it. This keeps pipe and connection data consistent.
bool GasNetwork::erasePipeById(int id) {
    auto pipeIt = std::find_if(pipes.begin(), pipes.end(),
        [id](const Pipe& pipe) { return pipe.id == id; });
    if (pipeIt == pipes.end()) return false;

    connections.erase(std::remove_if(connections.begin(), connections.end(),
        [id](const Connection& connection) { return connection.pipeId == id; }),
        connections.end());
    pipes.erase(pipeIt);
    return true;
}

void GasNetwork::deletePipe() {
    if (pipes.empty()) {
        std::cout << "No pipes to delete.\n";
        logAction("DELETE_PIPE_FAILED", "No pipes exist");
        return;
    }

    showNetwork();
    const int id = readInt("Enter pipe ID to delete: ");
    if (!erasePipeById(id)) {
        std::cout << "Pipe with such ID not found.\n";
        logAction("DELETE_PIPE_FAILED", "ID=" + std::to_string(id) + " not found");
        return;
    }

    std::cout << "Pipe " << id << " deleted. Any connection using it was removed.\n";
    logAction("DELETE_PIPE", "ID=" + std::to_string(id));
}

void GasNetwork::editStation() {
    if (stations.empty()) {
        std::cout << "No stations to edit.\n";
        logAction("EDIT_STATION_FAILED", "No stations exist");
        return;
    }

    for (const Station& station : stations) {
        std::cout << "ID: " << station.id << " | Name: " << station.name
                  << " | Workshops: " << station.activeNasos << "/" << station.totalNasos
                  << " | In repair: " << station.totalRepairWorksNasos << "\n";
    }

    const int id = readInt("Enter station ID to edit: ");
    Station* station = findStation(id);
    if (!station) {
        std::cout << "Station with such ID not found.\n";
        logAction("EDIT_STATION_FAILED", "ID=" + std::to_string(id) + " not found");
        return;
    }

    std::cout << "1. Change name\n"
              << "2. Change total number of workshops\n"
              << "3. Change number of active workshops\n"
              << "4. Change number of workshops in repair\n"
              << "0. Cancel\n";
    const int choice = readInt("Choose what to edit: ");
    switch (choice) {
        case 1:
            station->name = readString("Enter new station name: ");
            break;
        case 2: {
            const int total = readInt("Enter new total number of workshops: ");
            if (total <= 0 || total < station->activeNasos + station->totalRepairWorksNasos) {
                std::cout << "Total must be positive and not smaller than active + repair workshops.\n";
                logAction("EDIT_STATION_FAILED", "ID=" + std::to_string(id) + ", invalid total");
                return;
            }
            station->totalNasos = total;
            break;
        }
        case 3: {
            const int maxActive = station->totalNasos - station->totalRepairWorksNasos;
            const int active = readInt("Enter new number of active workshops: ");
            if (active < 0 || active > maxActive) {
                std::cout << "Active workshops must be between 0 and " << maxActive << ".\n";
                logAction("EDIT_STATION_FAILED", "ID=" + std::to_string(id) + ", invalid active count");
                return;
            }
            station->activeNasos = active;
            break;
        }
        case 4: {
            const int maxRepair = station->totalNasos - station->activeNasos;
            const int repair = readInt("Enter new number of workshops in repair: ");
            if (repair < 0 || repair > maxRepair) {
                std::cout << "Workshops in repair must be between 0 and " << maxRepair << ".\n";
                logAction("EDIT_STATION_FAILED", "ID=" + std::to_string(id) + ", invalid repair count");
                return;
            }
            station->totalRepairWorksNasos = repair;
            break;
        }
        case 0:
            logAction("EDIT_STATION_CANCELLED", "ID=" + std::to_string(id));
            std::cout << "Editing cancelled.\n";
            return;
        default:
            std::cout << "Unknown option.\n";
            logAction("EDIT_STATION_FAILED", "ID=" + std::to_string(id) + ", invalid option");
            return;
    }

    std::cout << "Station updated.\n";
    logAction("EDIT_STATION", "ID=" + std::to_string(id) + ", option=" + std::to_string(choice));
}

void GasNetwork::deleteStation() {
    if (stations.empty()) {
        std::cout << "No stations to delete.\n";
        logAction("DELETE_STATION_FAILED", "No stations exist");
        return;
    }

    for (const Station& station : stations) {
        std::cout << "ID: " << station.id << " | Name: " << station.name << "\n";
    }
    const int id = readInt("Enter station ID to delete: ");
    if (!findStation(id)) {
        std::cout << "Station with such ID not found.\n";
        logAction("DELETE_STATION_FAILED", "ID=" + std::to_string(id) + " not found");
        return;
    }
    if (!ask("Delete this station and its connections?")) {
        std::cout << "Deletion cancelled.\n";
        logAction("DELETE_STATION_CANCELLED", "ID=" + std::to_string(id));
        return;
    }

    std::vector<int> freedPipeIds;
    for (const Connection& connection : connections) {
        if (connection.fromStation == id || connection.toStation == id) {
            freedPipeIds.push_back(connection.pipeId);
        }
    }
    connections.erase(std::remove_if(connections.begin(), connections.end(),
        [id](const Connection& connection) {
            return connection.fromStation == id || connection.toStation == id;
        }), connections.end());

    for (int pipeId : freedPipeIds) {
        Pipe* pipe = findPipe(pipeId);
        if (pipe) pipe->isConnected = false;
    }
    stations.erase(std::remove_if(stations.begin(), stations.end(),
        [id](const Station& station) { return station.id == id; }), stations.end());

    std::cout << "Station " << id << " deleted. Related connections removed and their pipes freed.\n";
    logAction("DELETE_STATION", "ID=" + std::to_string(id) + ", removedConnections=" +
              std::to_string(freedPipeIds.size()));
}

// Interactive filter shared by pipe search and batch operations.
std::vector<int> GasNetwork::findPipesByUserFilter() {
    std::vector<int> matchedIds;
    if (pipes.empty()) {
        std::cout << "No pipes available.\n";
        return matchedIds;
    }

    std::cout << "Pipe filter:\n"
              << "1. By name (contains text)\n"
              << "2. By repair status\n"
              << "3. All pipes\n";
    const int filter = readInt("Choose filter: ");
    std::string namePart;
    int repairFilter = -1;
    if (filter == 1) {
        namePart = lowerCopy(readString("Enter part of pipe name: "));
    } else if (filter == 2) {
        repairFilter = readInt("In repair? (1 - yes, 0 - no): ");
        if (repairFilter != 0 && repairFilter != 1) {
            std::cout << "Invalid repair status.\n";
            logAction("PIPE_FILTER_FAILED", "invalid repair status");
            return matchedIds;
        }
    } else if (filter != 3) {
        std::cout << "Unknown filter.\n";
        logAction("PIPE_FILTER_FAILED", "invalid filter option=" + std::to_string(filter));
        return matchedIds;
    }

    for (const Pipe& pipe : pipes) {
        bool matches = false;
        if (filter == 1) {
            matches = lowerCopy(pipe.name).find(namePart) != std::string::npos;
        } else if (filter == 2) {
            matches = pipe.inRepair == (repairFilter == 1);
        } else {
            matches = true;
        }
        if (matches) matchedIds.push_back(pipe.id);
    }

    std::cout << "Matching pipes: " << matchedIds.size() << "\n";
    for (int id : matchedIds) {
        const Pipe* pipe = findPipe(id);
        if (pipe) {
            std::cout << "ID: " << pipe->id << " | Name: " << pipe->name
                      << " | Length: " << pipe->length << " km | Diameter: " << pipe->diameter
                      << " mm | Repair: " << (pipe->inRepair ? "Yes" : "No")
                      << " | Connected: " << (pipe->isConnected ? "Yes" : "No") << "\n";
        }
    }
    const std::string filterDescription = filter == 1 ? "name=" + namePart :
        (filter == 2 ? "inRepair=" + std::string(repairFilter == 1 ? "true" : "false") : "all pipes");
    logAction("SEARCH_PIPES", filterDescription + ", matches=" + std::to_string(matchedIds.size()));
    return matchedIds;
}

std::vector<int> GasNetwork::choosePipesFromMatches(const std::vector<int>& matchedIds) {
    std::vector<int> selected;
    if (matchedIds.empty()) return selected;

    std::cout << "1. Select all matching pipes\n"
              << "2. Select a subset by ID\n"
              << "0. Cancel\n";
    const int choice = readInt("Choose selection mode: ");
    if (choice == 0) return selected;
    if (choice == 1) return matchedIds;
    if (choice != 2) {
        std::cout << "Unknown selection mode.\n";
        return selected;
    }

    int count = readInt("How many pipes do you want to select? ");
    if (count <= 0 || static_cast<std::size_t>(count) > matchedIds.size()) {
        std::cout << "Invalid number of pipes.\n";
        return selected;
    }

    std::unordered_set<int> allowed(matchedIds.begin(), matchedIds.end());
    std::unordered_set<int> alreadySelected;
    for (int i = 0; i < count; ++i) {
        const int id = readInt("Enter selected pipe ID: ");
        if (!allowed.count(id)) {
            std::cout << "Pipe ID " << id << " is not in the search results. Try again.\n";
            --i;
            continue;
        }
        if (!alreadySelected.insert(id).second) {
            std::cout << "This ID has already been selected. Try again.\n";
            --i;
            continue;
        }
        selected.push_back(id);
    }
    return selected;
}

void GasNetwork::searchPipes() {
    if (pipes.empty()) {
        std::cout << "No pipes available for search.\n";
        logAction("SEARCH_PIPES_FAILED", "No pipes exist");
        return;
    }
    const std::vector<int> result = findPipesByUserFilter();
    if (result.empty()) std::cout << "No pipes matched the filter.\n";
}

void GasNetwork::searchStations() {
    if (stations.empty()) {
        std::cout << "No stations available for search.\n";
        logAction("SEARCH_STATIONS_FAILED", "No stations exist");
        return;
    }

    std::cout << "Station filter:\n"
              << "1. By name (contains text)\n"
              << "2. By percentage of unused workshops\n";
    const int filter = readInt("Choose filter: ");
    std::vector<const Station*> matches;
    std::string namePart;
    double minPercent = 0.0;
    double maxPercent = 100.0;

    if (filter == 1) {
        namePart = lowerCopy(readString("Enter part of station name: "));
        for (const Station& station : stations) {
            if (lowerCopy(station.name).find(namePart) != std::string::npos) {
                matches.push_back(&station);
            }
        }
    } else if (filter == 2) {
        auto readPercent = [](const std::string& prompt) {
            double value;
            while (true) {
                std::cout << prompt;
                if (std::cin >> value && value >= 0.0 && value <= 100.0) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    return value;
                }
                std::cout << "Enter a percentage between 0 and 100.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        };
        minPercent = readPercent("Minimum unused workshop percentage (0-100): ");
        maxPercent = readPercent("Maximum unused workshop percentage (0-100): ");
        if (minPercent > maxPercent) {
            std::cout << "Minimum percentage cannot be greater than maximum.\n";
            logAction("SEARCH_STATIONS_FAILED", "invalid percentage range");
            return;
        }
        for (const Station& station : stations) {
            const int freeShops = station.totalNasos - station.activeNasos - station.totalRepairWorksNasos;
            const double percent = station.totalNasos > 0
                ? 100.0 * freeShops / station.totalNasos : 0.0;
            if (percent >= minPercent && percent <= maxPercent) matches.push_back(&station);
        }
    } else {
        std::cout << "Unknown filter.\n";
        logAction("SEARCH_STATIONS_FAILED", "invalid filter option=" + std::to_string(filter));
        return;
    }

    std::cout << "Matching stations: " << matches.size() << "\n";
    for (const Station* station : matches) {
        const int freeShops = station->totalNasos - station->activeNasos - station->totalRepairWorksNasos;
        const double percent = station->totalNasos > 0
            ? 100.0 * freeShops / station->totalNasos : 0.0;
        std::cout << "ID: " << station->id << " | Name: " << station->name
                  << " | Active: " << station->activeNasos << "/" << station->totalNasos
                  << " | In repair: " << station->totalRepairWorksNasos
                  << " | Unused: " << percent << "%\n";
    }

    if (filter == 1) {
        logAction("SEARCH_STATIONS", "name=" + namePart + ", matches=" + std::to_string(matches.size()));
    } else {
        logAction("SEARCH_STATIONS", "unusedPercent=" + std::to_string(minPercent) + "-" +
                  std::to_string(maxPercent) + ", matches=" + std::to_string(matches.size()));
    }
}

void GasNetwork::batchEditPipes() {
    if (pipes.empty()) {
        std::cout << "No pipes available for batch operations.\n";
        logAction("BATCH_PIPE_OPERATION_FAILED", "No pipes exist");
        return;
    }

    const std::vector<int> matchedIds = findPipesByUserFilter();
    if (matchedIds.empty()) {
        std::cout << "No pipes matched the filter.\n";
        logAction("BATCH_PIPE_OPERATION_FAILED", "No matches");
        return;
    }
    const std::vector<int> selectedIds = choosePipesFromMatches(matchedIds);
    if (selectedIds.empty()) {
        std::cout << "Batch operation cancelled or no pipes selected.\n";
        logAction("BATCH_PIPE_OPERATION_CANCELLED", "No pipe selected");
        return;
    }

    std::cout << "Batch operation:\n"
              << "1. Set the same name for selected pipes\n"
              << "2. Set the same length for selected pipes\n"
              << "3. Set the same diameter for selected pipes\n"
              << "4. Toggle repair status of selected pipes\n"
              << "5. Delete selected pipes\n"
              << "0. Cancel\n";
    const int operation = readInt("Choose operation: ");
    if (operation == 0) {
        logAction("BATCH_PIPE_OPERATION_CANCELLED", "User cancelled");
        return;
    }

    if (operation == 1) {
        const std::string newName = readString("Enter the new name for all selected pipes: ");
        for (int id : selectedIds) {
            Pipe* pipe = findPipe(id);
            if (pipe) pipe->name = newName;
        }
    } else if (operation == 2) {
        const double newLength = readPositiveDoubles("Enter the new length in kilometers: ");
        for (int id : selectedIds) {
            Pipe* pipe = findPipe(id);
            if (pipe) pipe->length = newLength;
        }
    } else if (operation == 3) {
        const int newDiameter = readDiameter();
        for (int id : selectedIds) {
            Pipe* pipe = findPipe(id);
            if (pipe) pipe->diameter = newDiameter;
        }
    } else if (operation == 4) {
        for (int id : selectedIds) {
            Pipe* pipe = findPipe(id);
            if (pipe) pipe->inRepair = !pipe->inRepair;
        }
    } else if (operation == 5) {
        if (!ask("Delete all selected pipes?")) {
            std::cout << "Deletion cancelled.\n";
            logAction("BATCH_DELETE_PIPES_CANCELLED", "User rejected confirmation");
            return;
        }
        for (int id : selectedIds) {
            erasePipeById(id);
            logAction("DELETE_PIPE", "ID=" + std::to_string(id) + ", source=batch");
        }
        std::cout << "Selected pipes deleted.\n";
        logAction("BATCH_DELETE_PIPES", "count=" + std::to_string(selectedIds.size()));
        return;
    } else {
        std::cout << "Unknown operation.\n";
        logAction("BATCH_PIPE_OPERATION_FAILED", "invalid operation=" + std::to_string(operation));
        return;
    }

    std::cout << "Batch operation completed for " << selectedIds.size() << " pipe(s).\n";
    logAction("BATCH_EDIT_PIPES", "operation=" + std::to_string(operation) +
              ", count=" + std::to_string(selectedIds.size()));
}


void GasNetwork::saveData() const {
    const std::string filename = readString("Enter filename to save (for example network.txt): ");
    std::ofstream file(filename);
    if (!file) {
        std::cout << "Failed to open " << filename << " for writing\n";
        logAction("SAVE_FAILED", "file=" + filename + ", cannot open for writing");
        return;
    }

    file << "GAS_NETWORK_V1\n";
    file << pipes.size() << "\n";
    for (const Pipe& pipe : pipes) {
        file << pipe.id << ' ' << std::quoted(pipe.name) << ' '
             << std::setprecision(17) << pipe.length << ' '
             << pipe.diameter << ' ' << pipe.inRepair << ' '
             << pipe.isConnected << "\n";
    }

    file << stations.size() << "\n";
    for (const Station& station : stations) {
        file << station.id << ' ' << std::quoted(station.name) << ' '
             << station.totalNasos << ' ' << station.activeNasos << ' '
             << station.totalRepairWorksNasos << "\n";
    }

    file << connections.size() << "\n";
    for (const Connection& connection : connections) {
        file << connection.pipeId << ' ' << connection.fromStation << ' '
             << connection.toStation << "\n";
    }

    if (!file) {
        std::cout << "Error while writing data\n";
        logAction("SAVE_FAILED", "file=" + filename + ", write error");
        return;
    }
    std::cout << "Data saved to " << filename << "\n";
    logAction("SAVE", "file=" + filename + ", pipes=" + std::to_string(pipes.size()) +
              ", stations=" + std::to_string(stations.size()));
}

void GasNetwork::loadData() {
    const std::string filename = readString("Enter filename to load (for example network.txt): ");
    logAction("LOAD_ATTEMPT", "file=" + filename);
    std::ifstream file(filename);
    if (!file) {
        std::cout << "File " << filename << " not found\n";
        logAction("LOAD_FAILED", "file=" + filename + ", cannot open");
        return;
    }

    std::string signature;
    std::getline(file, signature);
    if (signature != "GAS_NETWORK_V1") {
        std::cout << "Invalid file format. Please use a file saved by this version of the program\n";
        logAction("LOAD_FAILED", "file=" + filename + ", invalid format");
        return;
    }

    std::vector<Pipe> loadedPipes;
    std::vector<Station> loadedStations;
    std::vector<Connection> loadedConnections;
    std::size_t count = 0;

    if (!(file >> count) || count > 100000) {
        std::cout << "Invalid data in file\n";
        logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
        return;
    }
    for (std::size_t i = 0; i < count; ++i) {
        Pipe pipe;
        if (!(file >> pipe.id >> std::quoted(pipe.name) >> pipe.length
                    >> pipe.diameter >> pipe.inRepair >> pipe.isConnected)) {
            std::cout << "Error reading pipe data\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
        if (pipe.id <= 0 || pipe.length <= 0 ||
            (pipe.diameter != 500 && pipe.diameter != 700 &&
             pipe.diameter != 1000 && pipe.diameter != 1400)) {
            std::cout << "Invalid pipe data found\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
        loadedPipes.push_back(pipe);
    }

    if (!(file >> count) || count > 100000) {
        std::cout << "Invalid data in file\n";
        logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
        return;
    }
    for (std::size_t i = 0; i < count; ++i) {
        Station station;
        if (!(file >> station.id >> std::quoted(station.name)
                    >> station.totalNasos >> station.activeNasos
                    >> station.totalRepairWorksNasos)) {
            std::cout << "Error reading station data\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
        if (station.id <= 0 || station.totalNasos <= 0 ||
            station.activeNasos < 0 || station.activeNasos > station.totalNasos ||
            station.totalRepairWorksNasos < 0 ||
            station.activeNasos + station.totalRepairWorksNasos > station.totalNasos) {
            std::cout << "Invalid station data found\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
        loadedStations.push_back(station);
    }

    if (!(file >> count) || count > 100000) {
        std::cout << "Invalid data in file\n";
        logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
        return;
    }
    for (std::size_t i = 0; i < count; ++i) {
        Connection connection;
        if (!(file >> connection.pipeId >> connection.fromStation
                    >> connection.toStation)) {
            std::cout << "Error reading connection data\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
        loadedConnections.push_back(connection);
    }

    std::unordered_set<int> pipeIds;
    std::unordered_set<int> stationIds;
    for (const Pipe& pipe : loadedPipes) {
        if (!pipeIds.insert(pipe.id).second) {
            std::cout << "Invalid pipe ID found in file\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
    }
    for (const Station& station : loadedStations) {
        if (!stationIds.insert(station.id).second) {
            std::cout << "Invalid station ID found in file\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
    }

    std::unordered_set<int> usedPipeIds;
    for (const Connection& connection : loadedConnections) {
        if (!pipeIds.count(connection.pipeId) ||
            !stationIds.count(connection.fromStation) ||
            !stationIds.count(connection.toStation) ||
            connection.fromStation == connection.toStation ||
            !usedPipeIds.insert(connection.pipeId).second) {
            std::cout << "Invalid connection found in file\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
    }

    for (Pipe& pipe : loadedPipes) {
        bool connectedByRecord = usedPipeIds.count(pipe.id) != 0;
        if (pipe.isConnected != connectedByRecord) {
            std::cout << "Pipe status does not match connection data\n";
            logAction("LOAD_FAILED", "file=" + filename + ", invalid or inconsistent data");
            return;
        }
    }

    pipes = std::move(loadedPipes);
    stations = std::move(loadedStations);
    connections = std::move(loadedConnections);
    std::cout << "Data successfully loaded from " << filename << "\n";
    logAction("LOAD", "file=" + filename + ", pipes=" + std::to_string(pipes.size()) +
              ", stations=" + std::to_string(stations.size()));
}
