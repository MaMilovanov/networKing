#ifndef HAT_H
#define HAT_H

#include <iostream>
#include <vector>

struct Pipe {
    int id = 0;
    std ::string name;
    double length=0.0;
    int diameter=500;
    bool inRepair=false;
    bool isConnected=false;
};

struct Station {
    int id =0;
    std::string name;
    int totalNasos=0;
    int activeNasos=0;
    int totalRepairWorksNasos=0;
};

struct Connection {
    int pipeId=0;
    int fromStation=0;
    int toStationId=0;
};

#endif