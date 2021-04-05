#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "road.hpp"
#include "area.hpp"

using namespace std;

int Area::sAreaCount = 0;

/*
int main() {
    vector<Road> rds = {Road("MVL"), Road("LBS"), Road("MG")};
    Area p = Area("Andheri", rds, HeavyTraffic);
    cout<<p<<endl<<p.IncludesRoad(Road("MVL"))<<endl<<p.IncludesRoad(Road("DEF"));
}*/