#ifndef SQLITEMANAGER_DATABASE_H
#define SQLITEMANAGER_DATABASE_H

#include <vector>
#include <string>
#include "../sqlite/sqlite3.h"
#include "Response.h"
#include "../config.h"

using namespace std;

class Database {
protected:
    sqlite3* db; // Pointer to the SQLite database

public:
    Database() : db(nullptr) {} // Constructor initializes db to nullptr

    // Create the database file
    static bool createDatabaseFile(const char* dbName = Config::DB_NAME) {
        sqlite3* db = nullptr; // Local database pointer
        // Attempt to open the database file
        if (sqlite3_open(dbName, &db) != SQLITE_OK) {
            return false; // Return false if unable to open
        }
        sqlite3_close(db); // Close the database connection
        return true; // Return true if successful
    }

    // Open the database connection
    pair<bool, const char*> startConnection() {
        // Attempt to open the database connection
        if (sqlite3_open(Config::DB_NAME, &db) != SQLITE_OK) {
            return {false, Response::ERROR_OPEN_DB}; // Return error response on failure
        }
        return {true, Response::CONNECTION_OPEN_SUCCESS}; // Return success response
    }

    // Fetch column names from a specified table
    pair<bool, vector<string>> getTableColumns(const string& tableName) {
        vector<string> columns; // Vector to hold column names
        sqlite3_stmt* stmt; // SQLite statement pointer
        string query = "PRAGMA table_info(" + tableName + ");"; // SQL query to fetch column info

        // Prepare the SQL statement
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return {false, columns}; // Return false if unable to prepare statement
        }

        // Execute the statement and fetch column names
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* columnName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); // Get column name
            if (columnName) {
                columns.push_back(columnName); // Add column name to vector
            }
        }

        // Finalize the statement
        if (sqlite3_finalize(stmt) != SQLITE_OK || columns.empty()) {
            return {false, columns}; // Return false if finalization fails or no columns found
        }

        return {true, columns}; // Return true and the list of column names
    }

    // Close the database connection
    pair<bool, const char*> closeConnection() {
        // Attempt to close the database connection
        if (db && sqlite3_close(db) != SQLITE_OK) {
            return {false, Response::ERROR_CLOSE_DB}; // Return error response on failure
        }
        db = nullptr; // Set db to nullptr after closing
        return {true, Response::CONNECTION_CLOSE_SUCCESS}; // Return success response
    }
};

#endif //SQLITEMANAGER_DATABASE_H
