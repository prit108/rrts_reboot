#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>

using namespace std;

#include "user.hpp"
#include "admin.hpp"
#include "complaint.hpp"
#include "city.hpp"
#include "area.hpp"
#include "road.hpp"

double Admin::sWater = 10000.00;

int Admin::sCementBags = 0;
int Admin::sSandBags = 0;
int Admin::sWorkers = 0;
int Admin::sMachines = 0;

void Admin::InitStatic() {
	fstream newfile;
	newfile.open("resources.txt", ios::in);
	if (newfile.is_open()) {
		newfile >> sCementBags;
		newfile >> sSandBags;
		newfile >> sWorkers;
		newfile >> sMachines;
		newfile.close();
	}
	else {
		cerr << "File containing resources data cannot be opened" << endl;
	}
}

string Admin::GetUserType() {
	return string("Admin");
}

bool compareComplaints(pair<Complaint,int> C1, pair<Complaint,int> C2) {
	Complaint c1 = C1.first;
	Complaint c2 = C2.first;
	//Priority assigned > type of area > task is pending or not > resources ( #workers > #machines > #sandbags > #cementbags)
	if (c1.GetPriority() < c2.GetPriority())
		return true;
	else if (City::Mumbai().GetArea(c1.GetRoad()).GetPriority() < City::Mumbai().GetArea(c1.GetRoad()).GetPriority())
		return true;
	else if (c1.IsPending() == true && c2.IsPending() == false)
		return true;
	else if (get<3>(c1.GetResources()) < get<3>(c2.GetResources()))//workers
		return true;
	else if (get<4>(c1.GetResources()) < get<4>(c2.GetResources()))//machines
		return true;
	else if (get<1>(c1.GetResources()) < get<1>(c2.GetResources()))//sandbags
		return true;
	else if (get<0>(c1.GetResources()) < get<0>(c2.GetResources()))//cementbags
		return true;
	else return false;
}

bool Admin::ResourcesAvailable(Complaint c){
	if (get<0>(c.GetResources()) > Admin::sCementBags || get<1>(c.GetResources()) > Admin::sSandBags || get<3>(c.GetResources()) > Admin::sWorkers || get<4>(c.GetResources()) > Admin::sMachines)
		return false;
	else return true;
}

vector<pair<Complaint,int> > Admin::Schedule(vector<Complaint>& comp) const {
	//Resources importance order: workers > machines > sand bags > cement bags
	int i, j, n;
	n = comp.size();
	vector<pair<Complaint, int>> compAlloc;
	for (i = 0; i < n; i++) {
		compAlloc.push_back(make_pair(comp[i], -1));
	}
	pair<int, int> morningResources;
	morningResources = make_pair(0, 0);
	sort(compAlloc.begin(), compAlloc.end(), compareComplaints);
	for (i = 0; i < n; i++) {
		if (ResourcesAvailable(compAlloc[i].first)) {
			compAlloc[i].second = 0; //alloted morning slot

			sCementBags -= get<0>(compAlloc[i].first.GetResources());
			sSandBags -= get<1>(compAlloc[i].first.GetResources());
			sWater -= get<2>(compAlloc[i].first.GetResources());

			sWorkers -= get<3>(compAlloc[i].first.GetResources());
			//freeing up workers allotted for only morning slot 
			if (get<5>(compAlloc[i].first.GetResources()) == 1)
				morningResources.first += get<0>(compAlloc[i].first.GetResources());

			sMachines -= get<4>(compAlloc[i].first.GetResources());
			//freeing up machines allotted for only morning slot 
			if (get<5>(compAlloc[i].first.GetResources()) == 1)
				morningResources.second += get<0>(compAlloc[i].first.GetResources());
		}
	}
	//updating resources for evening slot allocation
	sWorkers += morningResources.first;
	sMachines += morningResources.second;

	//starting next allocation
	for (i = 0; i < n; i++) {
		if (compAlloc[i].second==-1 && get<5>(compAlloc[i].first.GetResources())==1 && ResourcesAvailable(compAlloc[i].first)) {
			compAlloc[i].second = 1; //alloted evening slot

			sCementBags -= get<0>(compAlloc[i].first.GetResources());
			sSandBags -= get<1>(compAlloc[i].first.GetResources());
			sWater -= get<2>(compAlloc[i].first.GetResources());

			sWorkers -= get<3>(compAlloc[i].first.GetResources());

			sMachines -= get<4>(compAlloc[i].first.GetResources());
		}
	}

	//updating resources i.e., cement bags and sand bags in resources file
	fstream newfile;
	int a, b;
	newfile.open("resources.txt");
	if (newfile.is_open()) {
		newfile >> a;
		newfile >> b;
		newfile >> sWorkers;
		newfile >> sMachines;
		newfile << sCementBags;
		newfile << " ";
		newfile << sSandBags;
		newfile << " ";
		newfile << sWorkers;
		newfile << " ";
		newfile << sMachines;
		newfile << endl;
		newfile.close();
	}
	else {
		cerr << "File containing resources data cannot be opened for updation" << endl;
	}

	return compAlloc;
}
/*
// unit test
int main () {
	//Admin::CityAdmin().Schedule();
}*/