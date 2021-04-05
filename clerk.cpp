#include<bits/stdc++.h>
#include "user.hpp"
#include "complaint.hpp"

using namespace std;

class Clerk : public User
{
	public:
		static bool AddComplaintToDB(string, string);
		Clerk(){}

		string GetUserType() 
		{
			return ("Clerk");
		} 

		bool IsUser(int x,string s) {
			if((x==(this->userId_)) && (s == (this->password_)))
			return true;
			else
			return false;
		}

		void AddComplaints(Complaint c)
		{
 		  
		}

		~Clerk()
		{

		}
};