#ifndef CITY_HPP
#define CITY_HPP
#include <iostream>
#include <string>
#include <vector>
#include "road.hpp"
#include "area.hpp"

using namespace std;

class City {
    vector<Area> areas_;
    City(vector<Area> areas): 
    areas_(areas) {}
public:
    static vector<Area> sAreaList;
    ~City() {}
    static City& Mumbai() {
        static City theobj = City(City::sAreaList);
        return theobj;
    }
    Area GetArea(const Road& rd) const {
        for (Area x : this->areas_){
            if(x.IncludesRoad(rd))
                return x;
        }        
    }
    Road GetRoadObject(string rd) const{
        for (Area x : this->areas_){
            for(Road y : x.GetListRoads()) {
                if(y.ToString() == rd) {
                    return y;
                } 
            }
        }
    }
    Area GetAreaObject(string ar) const {
        for (Area x : this->areas_){
            if(x.GetName() == ar) {
                return x;
            }
        }
    }
    vector<Area> GetAreaList() const {
        return this->areas_;
    }
    friend ostream& operator<<(ostream& os, const City& city) {
        os << "City Area List : " << endl;
        for(Area x : city.areas_) {
            os << x << endl;
        }
        return os;
    }
};

#endif //CITY_HPP