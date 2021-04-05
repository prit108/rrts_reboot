#ifndef AREA_HPP
#define AREA_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "road.hpp"

using namespace std;

enum AreaType {HeavyTraffic, MediumTraffic, LowTraffic};

class Area {
    const string name_;
    const int id_;
    vector<Road> roads_;
    const AreaType type_;
    static int sAreaCount;
public:
    Area(const string& name, const vector<Road>& roads, const AreaType& type):
    name_(name), roads_(roads), type_(type), id_(Area::sAreaCount) {
        Area::sAreaCount++;
    }
    ~Area() {}
    int GetId() {
        return this->id_;
    }
    const string& GetName() {
        return this->name_;
    }
    int GetPriority() {
        return this->type_;
    }
    vector<Road> GetListRoads() {
        return this->roads_;
    }

    bool IncludesRoad(const Road& rd) {
        return !(find(this->roads_.begin(), this->roads_.end(), rd) == this->roads_.end());
    }
    friend ostream& operator<<(ostream& os, const Area& a) {
        os << "Area:" << a.name_ << ", ID:" <<a.id_<< ", Type:"<<a.type_;
        return os; 
    }
};

#endif //AREA_HPP