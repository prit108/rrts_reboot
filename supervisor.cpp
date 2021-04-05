#include<bits/stdc++.h>

#include "user.hpp"
#include "area.hpp"
#include "complaint.hpp"
using namespace std;

class Supervisor : public User
{
	vector <Area> assignedAreas;
	vector <Complaint> assignedComplaints;
	map<Complaint,int> priority;
	int size = 0; 
public:
	Supervisor() {}

	bool IsUser(int x,string s)
	{
		if((x==(this->userId_)) && (s == (this->password_)))
			return true;
		else
			return false;
	}

	string GetUserType() 
	{
		return ("Supervisor");
	} 

	void GetNewComplaint(Complaint c)
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

		} 

		map<Complaint, int>& assignPriority()
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
		}

	
		~Supervisor()
		{

		}

};