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
        User(n,uid,passwd){}
		string GetUserType();
		//map<Complaint,int>& assignPriority();
		void AddArea(Area p) {
			this->assignedAreas_.push_back(p);
		}
		vector<Area> GetAssignedAreas() {
			return this->assignedAreas_;
		}
		vector<Complaint> GetAssignedComplaints() {
			return this->assignedComplaint_;
		}
		void SetResourcesThisComplaint(string c, int priority, tuple <int, int, double, int, int, int> res){
			for(int i = 0; i < this->assignedComplaint_.size(); i++){
				if(this->assignedComplaint_[i].ToString() == c){
					this->assignedComplaint_[i].SetPriority(priority);
					this->assignedComplaint_[i].SetResources(res);
					return;
				}
			}
		}
		void SetComplaints(vector<Complaint> p);
		static string GetAreaSupervisor(vector<Supervisor>& p, Area ar);
		static bool GetAssignedAreaList(Supervisor& p);
		static bool PushResourcesToDB(Complaint& p);
		~Supervisor(){}
};
#endif