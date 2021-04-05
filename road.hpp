#ifndef ROAD_HPP
#define ROAD_HPP
#include <iostream>
#include <string>
using namespace std;

class Road {
    string name_;
    int id_;
    static int sRoadCount;
public:

    Road(const string& name):
    name_(name), id_(Road::sRoadCount) {
        Road::sRoadCount++;
    }

    Road(const Road& rd): name_(rd.name_), id_(rd.id_){}

    ~Road() {}

    int GetId () {
        return this->id_;
    }

    string ToString() {
        return this->name_ + " Road";
    }

    friend bool operator==(const Road& r1, const Road& r2) {
        return r1.name_ == r2.name_ ;
    }

    friend ostream& operator<<(ostream& os, const Road& p) {
        os << p.name_ << " Road " << " : Id " << p.id_;
        return os; 
    }
};

#endif //ROAD_HPP