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
        return;
    }

    int id = readInt("Enter pipe ID: ");
    Pipe* pipe = findPipe(id);
    if (!pipe) {
        std::cout << "Pipe with such ID not foun\n";
        return;
    }

    pipe->inRepair = !pipe->inRepair;
    std::cout << "Pipe state changed: "
              << (pipe->inRepair ? "in repair" : "not in repair") << ".\n";
}

void GasNetwork::manageStation() {
    if (stations.empty()) {
        std::cout << "First, create a station\n";
        return;
    }

    int id = readInt("Enter station ID: ");
    Station* station = findStation(id);
    if (!station) {
        std::cout << "Station with such ID not found\n";
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
        std::cout << "Nu ti dibil?\n";
    }
}

void GasNetwork::connectStations() {
    if (stations.size() < 2) {
        std::cout << "For connection, need to create at least two stations\n";
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
        return;
    }
    if (fromId == toId) {
        std::cout << "Cannot connect a station to itself\n";
        return;
    }

    int diameter = readDiameter();

    for (const Connection& connection : connections) {
        if (connection.fromStation == fromId &&
            connection.toStation == toId) {
            std::cout << "These stations are already connected\n";
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
}

void GasNetwork::sort() const {
    if (stations.empty()) {
        std::cout << "No stations available for sorting\n";
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
}

void GasNetwork::saveData() const {
    std::ofstream file("data.txt");
    if (!file) {
        std::cout << "Failed to open data.txt for writing\n";
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
        return;
    }
    std::cout << "Data saved to data.txt\n";
}

void GasNetwork::loadData() {
    std::ifstream file("data.txt");
    if (!file) {
        std::cout << "File data.txt not found\n";
        return;
    }

    std::string signature;
    std::getline(file, signature);
    if (signature != "GAS_NETWORK_V1") {
        std::cout << "Invalid file format. Please use a file saved by this version of the program\n";
        return;
    }

    std::vector<Pipe> loadedPipes;
    std::vector<Station> loadedStations;
    std::vector<Connection> loadedConnections;
    std::size_t count = 0;

    if (!(file >> count) || count > 100000) {
        std::cout << "Invalid data in file\n";
        return;
    }
    for (std::size_t i = 0; i < count; ++i) {
        Pipe pipe;
        if (!(file >> pipe.id >> std::quoted(pipe.name) >> pipe.length
                    >> pipe.diameter >> pipe.inRepair >> pipe.isConnected)) {
            std::cout << "Error reading pipe data\n";
            return;
        }
        if (pipe.id <= 0 || pipe.length <= 0 ||
            (pipe.diameter != 500 && pipe.diameter != 700 &&
             pipe.diameter != 1000 && pipe.diameter != 1400)) {
            std::cout << "Invalid pipe data found\n";
            return;
        }
        loadedPipes.push_back(pipe);
    }

    if (!(file >> count) || count > 100000) {
        std::cout << "Invalid data in file\n";
        return;
    }
    for (std::size_t i = 0; i < count; ++i) {
        Station station;
        if (!(file >> station.id >> std::quoted(station.name)
                    >> station.totalNasos >> station.activeNasos
                    >> station.totalRepairWorksNasos)) {
            std::cout << "Error reading station data\n";
            return;
        }
        if (station.id <= 0 || station.totalNasos <= 0 ||
            station.activeNasos < 0 || station.activeNasos > station.totalNasos ||
            station.totalRepairWorksNasos < 0 ||
            station.activeNasos + station.totalRepairWorksNasos > station.totalNasos) {
            std::cout << "Invalid station data found\n";
            return;
        }
        loadedStations.push_back(station);
    }

    if (!(file >> count) || count > 100000) {
        std::cout << "Invalid data in file\n";
        return;
    }
    for (std::size_t i = 0; i < count; ++i) {
        Connection connection;
        if (!(file >> connection.pipeId >> connection.fromStation
                    >> connection.toStation)) {
            std::cout << "Error reading connection data\n";
            return;
        }
        loadedConnections.push_back(connection);
    }

    std::unordered_set<int> pipeIds;
    std::unordered_set<int> stationIds;
    for (const Pipe& pipe : loadedPipes) {
        if (!pipeIds.insert(pipe.id).second) {
            std::cout << "Invalid pipe ID found in file\n";
            return;
        }
    }
    for (const Station& station : loadedStations) {
        if (!stationIds.insert(station.id).second) {
            std::cout << "Invalid station ID found in file\n";
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
            return;
        }
    }

    for (Pipe& pipe : loadedPipes) {
        bool connectedByRecord = usedPipeIds.count(pipe.id) != 0;
        if (pipe.isConnected != connectedByRecord) {
            std::cout << "Pipe status does not match connection data\n";
            return;
        }
    }

    pipes = std::move(loadedPipes);
    stations = std::move(loadedStations);
    connections = std::move(loadedConnections);
    std::cout << "Data successfully loaded from data.txt\n";
}