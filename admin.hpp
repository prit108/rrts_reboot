#ifndef ADMIN_HPP
#define ADMIN_HPP
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>

using namespace std;

#include "user.hpp"
#include "complaint.hpp"
#include "city.hpp"
#include "area.hpp"
#include "road.hpp"

class Admin : public User {
	
	Admin(string n, int uid, string p) : User(n, uid, p) {}
	~Admin() {}
	static int sCementBags;
	static int sSandBags;
	static double sWater;
	static int sWorkers;
	static int sMachines;
	static void InitStatic();
	
public:
	static const Admin& CityAdmin(string n, int uid, string p) {
		static Admin theObject(n,uid,p);
		InitStatic();
		return theObject;
	}
	/*bool IsUser(int x, string s) {
		return true;
		//To be changed later
	}*/
	string GetUserType();
	static bool ResourcesAvailable(Complaint c);
	vector<pair<Complaint,int> > Schedule(vector<Complaint>& comp) const;
	void ModifyResources() const;
	void ChangeAreaAlloc() const;
	void AddSupervisor() const;
};

#endif //ADMIN_HPP