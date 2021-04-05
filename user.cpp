#include <iostream>
#include <string>
#include "user.hpp"
#include <sqlite3.h>

using namespace std;

void User::SetPassword (const string p) {
    this->password_ = p;
}

int User::password_callback(void* data, int argc, char** argv, char** azColName)
{
    string* retrieveinfo = static_cast<string*>(data);
    retrieveinfo[0] = string(argv[2]);
    retrieveinfo[1] = string(argv[3]);
    //cerr<<retrieveinfo[0]<<"  "<<retrieveinfo[1];
    /*
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
  
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    */
    return 0;
}
  
bool User::IsValidLogin(int userid, const string& password, string& type, string& name)
{
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("./datafiles/logindb.db", &DB);
    string data[2];
  
    string sql("SELECT * FROM userlogin WHERE userid == " + to_string(userid));
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (-1);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;
  
    int rc = sqlite3_exec(DB, sql.c_str(), password_callback, &data, NULL);
  
    if (rc != SQLITE_OK)
        cerr << "Error SELECT" << endl;
    else {
        cout << "Operation OK!" << endl;
        type = data[1];
        //cerr<<type<<" "<<password;
        if(data[0] == password) {
            return 1;
        }
        else {
            return 0;
        }
    }
  
    sqlite3_close(DB);
    return 0;
}

bool User::UpdatePassword(int userid, const string& password) {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("./datafiles/logindb.db", &DB);
    string data[2];
  
    string sql("UPDATE userlogin set password = "+ password +" WHERE userid == " + to_string(userid));
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
        return (0);
    }
    else
        std::cout << "Opened Database Successfully!" << std::endl;
  
    int rc = sqlite3_exec(DB, sql.c_str(), password_callback, &data, NULL);
    sqlite3_close(DB);
    if (rc != SQLITE_OK)
        cerr << "Error Update Password" << endl;
    else {
        cout << "Operation OK!" << endl;
        return 1;
    }
    return 0;
}