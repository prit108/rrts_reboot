#ifndef CLERK_HPP
#define CLERK_HPP

#include<bits/stdc++.h>
#include "complaint.hpp"
#include "user.hpp"
using namespace std;

class Clerk : public User
{
	public:
		Clerk(const string& n, const int& uid, const string& passwd):
        User(n,uid,passwd){}
		string GetUserType();
		~Clerk() {}
        static bool AddComplaintToDB(Complaint p);
};

#endif //CLERK_HPP