#ifndef MAYOR_CPP
#define MAYOR_CPP

#include <iostream>
#include <string>
using namespace std;

#include "user.hpp"
#include "complaint.hpp"
#include "city.hpp"
#include "area.hpp"
#include "road.hpp"

class Mayor: public User{
		Mayor(const string& n,const int& uid, const string& p): User(n,uid,p) {}
        ~Mayor() {}

public:
    static const Mayor& CityMayor(string n, int uid, string p) {
		static Mayor theObject(n,uid,p);
		return theObject;
	}
    bool IsUser(int x, string s) {
		return true;
		/*To be changed later*/
	}
	string GetUserType();
	void computestats();
};

#endif //MAYOR_CPP