#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "user.hpp"
#include "area.hpp"
#include "complaint.hpp"
using namespace std;

class Supervisor : public User
{
	private:
		vector<Area> assignedAreas_;
		vector<Complaint> assignedComplaint_;
		map<Complaint,int> priority_;
		int size = 0; 
	public:
		Supervisor(const string& n,const int& uid, const string& passwd):
        User(n,uid,passwd){
		}
		string GetUserType();
		map<Complaint,int>& assignPriority();
		void AddArea(Area p) {
			this->assignedAreas_.push_back(p);
		}
		vector<Area> GetAssignedAreas() {
			return this->assignedAreas_;
		}
		vector<Complaint> GetAssignedComplaints() {
			return this->assignedComplaint_;
		}
		Complaint GetThisComplaint(string c){
			for(Complaint x : this->assignedComplaint_){
				if(x.ToString() == c)
					return Complaint(x);
			}
		}

		static bool GetAssignedAreaList(Supervisor& p);
		static bool PushResourcesToDB(Complaint& p);
		void SetComplaints(vector<Complaint>);
		~Supervisor(){}
};

#endif