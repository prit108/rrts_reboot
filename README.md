### Road Repair Tracking System

A Road Repair Tracking System written in C++.
GTK GUI with gtkmm C++ Support.
SQLite3 Database Support.
GITHUB Repository : 

Application Executable File : rrts_app

Compile :
    -- Requires :
        gtkmm and its dependencies
        g++ for C++17
        sqlite3.h files for C/C++
    
    -- Command :
    g++ application.cpp user.cpp road.cpp city.cpp area.cpp complaint.cpp clerk.cpp admin.cpp supervisor.cpp `pkg-config --cflags --libs gtkmm-3.0` -o rrts_app -l sqlite3
