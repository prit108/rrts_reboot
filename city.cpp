#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "road.hpp"
#include "area.hpp"
#include "city.hpp"
using namespace std;

vector<Area> InitCityArea(const string& path) {
    map<string,AreaType> dictionary;
    dictionary["HeavyTraffic"] = HeavyTraffic;
    dictionary["MediumTraffic"] = MediumTraffic;
    dictionary["LowTraffic"] = LowTraffic;
    ifstream filein;
    filein.open(path);
    if (!filein.is_open()) {
        cerr<<"FATAL ERROR :: \'areas.txt\' cannot be opened." << endl;
        exit(1);
    }
    else {
        vector<Area> areas;
        string str;
        bool area_read = false;
        while(getline(filein, str)){
            if(str == "+" && !area_read) {
                area_read = true;
                continue;
            }
            if(area_read) {
                vector<Road> rds;
                string area_name = str;
                area_read = false;
                getline(filein, str);
                string area_type = str;
                getline(filein, str);
                if(str == "$") {
                    getline(filein, str);
                    while(str != "$") {
                        rds.push_back(Road(str));
                        //cout<<str<<endl;
                        getline(filein,str);
                    }
                }
                areas.push_back(Area(area_name, rds, dictionary[area_type]));
            }
        }
        return areas;
    }
}
const string citydatapath = "./datafiles/areas.txt";
vector<Area> City::sAreaList = InitCityArea(citydatapath);
/*
int main () {
    cout << City::Mumbai() << endl;
    cout << City::Mumbai().GetArea(Road("Perry"));
}*/