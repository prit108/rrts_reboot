#include <bits/stdc++.h>
#include "user.hpp"
#include "area.hpp"
#include "city.hpp"
#include "complaint.hpp"
#include "supervisor.hpp"
#include <sqlite3.h>
using namespace std;

string Supervisor::GetUserType() 
{
	return ("Supervisor");
} 

/*void Supervisor::GetNewComplaint(Complaint c)
	{
		assignedComplaints.push_back(c);
		int c_bag,s_bags,workers,machines,slots; double water;
	 	tuple <int, int, double, int, int, int>r; 

		cout<<"For complaint "<<size+1<<'\n';
		cout<<"Give quantity of cement bags"<<'\n';
		cin >> c_bag;
		cout<<"Give quantity of sand bags"<<'\n';
		cin>>s_bags;
		cout<<"Give quantity of water"<<'\n';
		cin >>water;
		cout<<"Give quantity of workers"<<'\n';
		cin >> workers;
		cout<<"Give quantity of machines"<<'\n';
		cin>>machines;
		cout<<"Give quantity of slots"<<'\n';
		cin >> slots;
		r = make_tuple(c_bag,s_bags,water,workers,machines,slots);
		c.SetResources(r);

		this->size++;

		} */
/*
map<Complaint, int>& Supervisor::assignPriority()
{
	int p; 
	cout<<"Give priority of each complaint between 1 and "<<size<<" "<<'\n';
	for(int i=0;i<this->size;i++)
	{
		p = 0;
		cout<<"Complaint "<< i+1<<'\n';
		cin >> p;

		this->priority[assignedComplaints[i]] = p;;
	}
	return this->priority;
}*/

void Supervisor::SetComplaints(vector<Complaint> p) {
	for(Complaint x : p) {
		if(count(this->assignedAreas_.begin(), this->assignedAreas_.end() ,City::Mumbai().GetArea(x.GetRoad()))){
			this->assignedComplaint_.push_back(Complaint(x));
		}
	}
	return;
}

int sup_callback(void* data, int argc, char** argv, char** azColName)
{
    string* retrieveinfo = static_cast<string*>(data);
    cerr<<argc<<" "<<argv[0]<<endl;
    for (int i = 1; i < argc; i++) {
        retrieveinfo[i-1] = argv[i] ? argv[i] : "NULL";
    }
    //cerr<<retrieveinfo[0]<<"  "<<retrieveinfo[1];
    /*
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
  
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    */
    return 0;
}

bool Supervisor::GetAssignedAreaList(Supervisor& p) {
    cerr<<"in GETASSIGNEDAREALIST"<<endl;
	sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("./datafiles/supervisor_area_db.db", &DB);
    string data[6] = {"", "", "", "", "", ""};
  
    string sql("SELECT * FROM areaalloc WHERE supervisorId == " + to_string(p.GetID()) +";");
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;
  
    int rc = sqlite3_exec(DB, sql.c_str(), sup_callback, &data, NULL);
  
    if (rc != SQLITE_OK)
        cerr << "Error SELECT" << endl;
    else {
        cout << "Operation OK!" << endl;
		for(int i = 0; i < 5; i++) {
			if(data[i] != "NULL") {
				p.AddArea(City::Mumbai().GetAreaObject(data[i]));
			}
		}
		return 1;
    }
  
    sqlite3_close(DB);
    return 0;
}


bool Supervisor::PushResourcesToDB(Complaint& p){
	sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("./datafiles/complaints.db", &DB);
    string data[6];
  
    string sql("UPDATE freshcomplaints SET cement = " + to_string(get<0>(p.GetResources())) +", sand = " + to_string(get<1>(p.GetResources())) + ", labor = " + to_string(get<3>(p.GetResources())) + ", machine = " + to_string(get<4>(p.GetResources())) + ", slot = " + to_string(get<5>(p.GetResources())) + ", priority = " + to_string(p.GetPriority()) + " WHERE id == "+to_string(p.GetId()) + ";");
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;
  
    int rc = sqlite3_exec(DB, sql.c_str(), sup_callback, &data, NULL);
  
    if (rc != SQLITE_OK)
        cerr << "Error SELECT" << endl;
    else {
        cout << "Operation OK!" << endl;
		return 1;
    }
  
    sqlite3_close(DB);
    return 0;
}

Supervisor Supervisor::GetAreaSupervisor(vector<Supervisor>& p, Area ar) {
    for(Supervisor x : p) {
        if(find(x.GetAssignedAreas().begin(), x.GetAssignedAreas().end(), ar) != x.GetAssignedAreas().end()){
            return x;
        }
    }
}