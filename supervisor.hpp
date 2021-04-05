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
		map<Complaint,int>& assignPriority();
		
		~Supervisor(){}
};

#endif