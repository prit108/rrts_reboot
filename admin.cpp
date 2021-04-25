#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <sqlite3.h>
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
	newfile.open("./datafiles/resources.txt", ios::in);
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
	else if (City::Mumbai().GetArea(c1.GetRoad()).GetPriority() < City::Mumbai().GetArea(c2.GetRoad()).GetPriority())
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

bool Admin::ResourcesAvailable(Complaint c) {
	if (get<0>(c.GetResources()) > Admin::sCementBags || get<1>(c.GetResources()) > Admin::sSandBags || get<3>(c.GetResources()) > Admin::sWorkers || get<4>(c.GetResources()) > Admin::sMachines)
		return false;
	else return true;
}

vector<pair<Complaint,int> > Admin::Schedule(vector<Complaint>& comp){
	//Resources importance order: workers > machines > sand bags > cement bags
	InitStatic();
	int i, j, n;
	n = comp.size();
	vector<pair<Complaint, int> > compAlloc;
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
				morningResources.first += get<3>(compAlloc[i].first.GetResources());

			sMachines -= get<4>(compAlloc[i].first.GetResources());
			//freeing up machines allotted for only morning slot 
			if (get<5>(compAlloc[i].first.GetResources()) == 1)
				morningResources.second += get<4>(compAlloc[i].first.GetResources());
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
	newfile.open("./datafiles/resources.txt");
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
		InitStatic();
	}
	else {
		cerr << "File containing resources data cannot be opened for updation" << endl;
	}

	return compAlloc;
}
sqlite3* db;

int select1_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
  vector<Complaint>* records = static_cast<vector<Complaint>*>(p_data);
  try {
	cerr<<"CALL BACK 1"<<endl;
    int id = atoi(p_fields[0]);
	cerr<<"fine1"<<endl;
	string road = string(p_fields[1]);
	cerr<<"fine2"<<endl;
	int cement = p_fields[2] == "" ? -1 : atoi(p_fields[2]);
	int sand = p_fields[3] == "" ? -1 : atoi(p_fields[3]);
	int labor = p_fields[4] == "" ? -1 : atoi(p_fields[4]);
	int machine = p_fields[5] == "" ? -1 : atoi(p_fields[5]);
	int slot = p_fields[6] == "" ? -1 : atoi(p_fields[6]);
	int priority = p_fields[7] == "" ? -1 : atoi(p_fields[7]);
	cerr<<"fine3"<<endl;
	string matter = string(p_fields[8]);
	cerr<<"fine4"<<endl;
	records->push_back(Complaint(id, City::Mumbai().GetRoadObject(road), matter, make_tuple(cement,sand,0,labor,machine,slot), priority, false));
	cerr<<"fine5"<<endl;
  }
  catch (...) {
    return 1;
  }
  return 0;
}

int select2_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
  vector<Complaint>* records = static_cast<vector<Complaint>*>(p_data);
  try {
    int id = atoi(p_fields[0]);
	string road = (p_fields[1]);
	int cement = atoi(p_fields[2]);
	int sand = atoi(p_fields[3]);
	int labor = atoi(p_fields[4]);
	int machine = atoi(p_fields[5]);
	int slot = atoi(p_fields[6]);
	int priority = atoi(p_fields[7]);
	string matter = p_fields[8];
	records->push_back(Complaint(id, City::Mumbai().GetRoadObject(road), matter, make_tuple(cement,sand,0.0,labor,machine,slot), priority,true));
  }
  catch (...) {
    return 1;
  }
  return 0;
}

vector<Complaint> select_stmt(const char* stmt, int p)
{
  vector<Complaint> records;  
  char *errmsg;
  int ret;
  if(p)
  ret = sqlite3_exec(db, stmt, select1_callback, &records, &errmsg);
  else 
  ret = sqlite3_exec(db, stmt, select2_callback, &records, &errmsg);
  if (ret != SQLITE_OK) {
    std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
  }
  else {
    std::cerr << records.size() << " records returned.\n";
  }

  return records;
}

void sql_stmt(const char* stmt)
{
  char *errmsg;
  int ret = sqlite3_exec(db, stmt, 0, 0, &errmsg);
  if (ret != SQLITE_OK) {
    std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
  }
}

bool Admin::GetTodayComplaint(vector<Complaint>& today) {
	if (sqlite3_open("./datafiles/complaints.db", &db) != SQLITE_OK) {
    std::cerr << "Could not open database.\n";
    return 0;
  	}
	cerr<<"Opened Database"<<endl;

  	vector<Complaint> records1 = select_stmt("SELECT * FROM freshcomplaints;",1);
	cerr<<"Fetched frshcomplaints"<<endl;
	sql_stmt("DELETE FROM freshcomplaints;");
	cerr<<"Cleared frsh list"<<endl;
	vector<Complaint> records2 = select_stmt("SELECT * FROM pendingcomplaints;", 0);
	cerr<<"Fetched pendcomplaints"<<endl;
	sql_stmt("DELETE FROM pendingcomplaints;");
	cerr<<"Cleared pend list"<<endl;

	for(Complaint x : records1) {
		today.push_back(x);
	}
	for(Complaint x : records2) {
		today.push_back(x);
	}
  	sqlite3_close(db);
	  return 1;
}


vector<pair<Complaint, int> > Admin::ModifyResources(int cbags, int sbags, int workers, int machines, vector<Complaint>& comp) const{
	//updating resources in resources file
	fstream newfile;
	newfile.open("./datafiles/resources.txt");
	if (newfile.is_open()) {
		newfile << cbags;
		newfile << " ";
		newfile << sbags;
		newfile << " ";
		newfile << workers;
		newfile << " ";
		newfile << machines;
		newfile << endl;
		newfile.close();
		InitStatic();
		return Schedule(comp);
	}
	else {
		cerr << "File containing resources data cannot be opened for updation" << endl;
	}
}

/*void Admin::UnitTestSchedule() {
	//Test case 1
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c1.SetPriority(1);
	c1.SetResources(make_tuple(1,1,1,1,1,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c2.SetPriority(2);
	c2.SetResources(make_tuple(1,1,1,1,1,1));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId())
		cout<<"Correct Schedule obtained in Test case 1"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 1"<<endl;

	//Test case 2
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[1].GetListRoads()[0], "bad condition", false);
	c1.SetPriority(1);
	c1.SetResources(make_tuple(1,1,1,1,1,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c2.SetPriority(1);
	c2.SetResources(make_tuple(1,1,1,1,1,1));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId())
		cout<<"Correct Schedule obtained in Test case 2"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 2"<<endl;

	//Test case 3
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", true);
	c1.SetPriority(1);
	c1.SetResources(make_tuple(1,1,1,1,1,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c2.SetPriority(1);
	c2.SetResources(make_tuple(1,1,1,1,1,1));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId())
		cout<<"Correct Schedule obtained in Test case 3"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 3"<<endl;

	//Test case 4
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", true);
	c1.SetPriority(1);
	c1.SetResources(make_tuple(1,1,1,1,1,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", true);
	c2.SetPriority(1);
	c2.SetResources(make_tuple(2,1,1,1,2,1));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId())
		cout<<"Correct Schedule obtained in Test case 4"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 4"<<endl;

	//Test case 5
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c1.SetPriority(1);
	c1.SetResources(make_tuple(1,1,1,1,3,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c2.SetPriority(2);
	c2.SetResources(make_tuple(1,1,1,1,1,2));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId() && schedule[0].second==0 && schedule[1].second==-1)
		cout<<"Correct Schedule obtained in Test case 5"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 5"<<endl;

	//Test case 6
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c1.SetPriority(1);
	c1.SetResources(make_tuple(30,1,1,1,1,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c2.SetPriority(2);
	c2.SetResources(make_tuple(30,1,1,1,1,1));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId() && schedule[0].second==0 && schedule[1].second==-1)
		cout<<"Correct Schedule obtained in Test case 6"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 6"<<endl;

	//Test case 7
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c1.SetPriority(2);
	c1.SetResources(make_tuple(20,1,1,1,1,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", false);
	c2.SetPriority(1);
	c2.SetResources(make_tuple(40,1,1,1,1,1));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId() && schedule[0].second==0 && schedule[1].second==-1)
		cout<<"Correct Schedule obtained in Test case 7"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 7"<<endl;

	//Test case 8
	Complaint c1 = Complaint((City::Mumbai().GetAreaList())[1].GetListRoads()[0], "bad condition", false);
	c1.SetPriority(1);
	c1.SetResources(make_tuple(2,1,1,1,1,1));
	Complaint c2 = Complaint((City::Mumbai().GetAreaList())[0].GetListRoads()[0], "bad condition", true);
	c2.SetPriority(1);
	c2.SetResources(make_tuple(2,1,1,1,1,1));
	Complaint c3 = Complaint((City::Mumbai().GetAreaList())[1].GetListRoads()[0], "bad condition", false);
	c3.SetPriority(2);
	c3.SetResources(make_tuple(2,2,1,1,1,1));
	vector<Complaint> comp;
	comp.push_back(c1);
	comp.push_back(c2);
	comp.push_back(c3);
	vector<pair<Complaint, int> > schedule = Admin::Schedule(comp);
	if(schedule[0].first.GetId()==c1.GetId() && schedule[1].first.GetId()==c2.GetId() && schedule[2].first.GetId()==c3.GetId())
		cout<<"Correct Schedule obtained in Test case 8"<<endl;
	else cerr<<"Wrong schedule obtained in Test case 8"<<endl;
}*/