cmake_minimum_required(VERSION 3.28)
project(SQLiteManager)

set(CMAKE_CXX_STANDARD 17)

add_executable(SQLiteManager main.cpp
        include/Response.h
        include/Database.h
        include/SQLiteManager.h
        config.h
        sqlite/sqlite3.c
        sqlite/sqlite3.h
        sqlite/sqlite3ext.h
)
