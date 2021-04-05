#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include "road.hpp"
#include "complaint.hpp"
using namespace std;



Complaint::Complaint(const int id, const Road& road, const string& matter, const bool isPending = false):
id_(id), road_(road), matter_(matter), resources_(make_tuple(0,0,0,0,0,0)), isPending_(isPending) {
}