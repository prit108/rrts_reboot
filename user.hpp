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
		User(string n,int uid, string p): name_(n),userId_(uid),password_(p) {}
		virtual ~User() {}
		string GetName() {
			return this->name_;
		}
		int GetID() {
			return this->userId_;
		}
		string GetPassword() {
			return this->password_;
		}
		//virtual bool IsUser(int x,string s);
		virtual string GetUserType() = 0;
		void SetPassword(string p);
		static bool IsValidLogin(int userid, const string& password, string& type, string& name);
		static bool UpdatePassword(int userid, const string& password);
		static int password_callback(void* data, int argc, char** argv, char** azColName);
};

#endif