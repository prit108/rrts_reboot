#include<bits/stdc++.h>
#include "user.hpp"
#include "clerk.hpp"
#include <sqlite3.h>
using namespace std;

string Clerk::GetUserType() 
{
	return ("Clerk");
} 


int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


bool Clerk::AddComplaintToDB(Complaint p) {
	sqlite3* DB;
    int exit = 0;
	char *zErrMsg = 0;
    exit = sqlite3_open("./datafiles/complaints.db", &DB);
	
  
    string sql("INSERT INTO freshcomplaints(id, road, matter) VALUES (" + to_string(p.GetId()) + ", \"" + p.GetRoad().ToString() + "\", \"" + p.GetComplaintMatter() +"\");");
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;
  
    int rc = rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &zErrMsg);
  
    if (rc != SQLITE_OK){
        cerr << "Error Update Complaints" << endl;
		sqlite3_free(zErrMsg);
	}
    else {
        cout << "Operation add complaint OK!" << endl;
		return 1;
    }
  
    sqlite3_close(DB);
    return 0;
}


  