#include <iostream>
#include <string>
#include <fstream>
#include <tuple>
#include "road.hpp"
#include "complaint.hpp"
using namespace std;

int Complaint::sComplaintCount = 0;

Complaint::Complaint(const Road& road, const string& matter, const bool isPending):
road_(road), matter_(matter), resources_(make_tuple(0,0,0,0,0,0)), isPending_(isPending) {
    fstream countfile;
    countfile.open("./datafiles/complaintcount.txt");
    if(!countfile.is_open()) {
        cerr<<"FATAL ERROR:: complaintcount.txt cannnot be opened."<<endl;
        exit(1);
    }
    int cnt;
    countfile>>cnt;
    cnt++;
    this->id_ = cnt;
    countfile.close();
    fstream changecnt;
    changecnt.open("./datafiles/complaintcount.txt", ios::trunc);
    if(!changecnt.is_open()) {
        cerr<<"FATAL ERROR:: complaintcount.txt cannnot be opened."<<endl;
        exit(1);
    }
    changecnt<<cnt;
    Complaint::sComplaintCount = cnt;
    changecnt.close();
}

Complaint::Complaint(const int id, const Road& road, const string& matter, const bool isPending):
id_(id), road_(road), matter_(matter), resources_(make_tuple(0,0,0,0,0,0)), isPending_(isPending) {}