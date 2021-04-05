#ifndef USER_HPP
#define USER_HPP

#include<iostream>
#include<string.h>
using namespace std;

class User
{
	protected: 
		string name_;
		int userId_;
		string password_;

	public: 
		User(string n,int uid,string p): name_(n),userId_(uid),password_(p) {}
		~User() {}
		const string& GetName() {
			return this->name_;
		}
		int GetID() {
			return this->userId_;
		}
		virtual bool IsUser(int x,string s) = 0;
		virtual string GetUserType() = 0;
		void SetPassword(string p);
		static bool IsValidLogin(int userid, const string& password, string& type, string& name);
		static bool UpdatePassword(int userid, const string& password);
};

#endif