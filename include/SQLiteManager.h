#ifndef SQLITEMANAGER_SQLITEMANAGER_H
#define SQLITEMANAGER_SQLITEMANAGER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include "Database.h"
#include "Response.h"

class SQLiteManager : public Database {
public:
    // Insert values into a specified table
    pair<bool, const char*> insertTo(const string& table, unordered_map<string, string>& values) {
        // Fetch the columns of the specified table
        auto columnsResult = this->getTableColumns(table);
        if (!columnsResult.first) {
            return {false, Response::ERROR_TABLE_NOT_FOUND}; // Return error if table is not found
        }

        const vector<string>& columns = columnsResult.second; // Get the list of columns
        string query = "INSERT INTO " + table + " ("; // Start building the insert query
        string valuesPart = "VALUES ("; // Start building the values part of the query
        bool first = true;

        // Construct the insert query dynamically based on provided values
        for (const string& column : columns) {
            if (values.find(column) != values.end()) { // Check if the column exists in the provided values
                if (!first) {
                    query += ", "; // Add comma for subsequent columns
                    valuesPart += ", "; // Add comma for values part
                }
                query += column; // Add column name to query
                valuesPart += "'" + values[column] + "'"; // Add corresponding value
                first = false; // Set first to false after the first column
            }
        }
        query += ") " + valuesPart + ");"; // Complete the insert query

        char* errMsg = nullptr; // Error message pointer
        if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            string error = Response::ERROR_INSERT; // Handle any insert errors
            error += ": ";
            error += errMsg; // Append error message
            sqlite3_free(errMsg); // Free the error message
            return {false, error.c_str()}; // Return failure
        }

        return {true, Response::INSERT_SUCCESS}; // Return success if insert is successful
    }

    // Find a record in the specified table by its ID
    pair<bool, const char*> find(const string& table, const string& id) {
        string query = "SELECT * FROM " + table + " WHERE id = '" + id + "';"; // SQL query to find the record

        sqlite3_stmt* stmt; // Statement pointer
        // Prepare the SQL statement
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return {false, Response::ERROR_FETCH}; // Return error if preparation fails
        }

        bool found = false; // Flag to indicate if record is found
        const char* result = nullptr; // Result pointer

        // Execute the statement and check for results
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            found = true; // Record found
            result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)); // Get the first column's value
        }

        sqlite3_finalize(stmt); // Finalize the statement

        if (!found) {
            return {false, Response::ERROR_RECORD_NOT_FOUND}; // Return error if record is not found
        }

        return {true, result}; // Return found record
    }

    // Fetch multiple records from a specified table with an optional limit
    pair<bool, vector<string>> fetch(const string& table, int limit = 0) {
        string query = "SELECT * FROM " + table; // Base SQL query to fetch records
        if (limit > 0) {
            query += " LIMIT " + to_string(limit); // Add limit to the query if specified
        }
        query += ";"; // Complete the query

        sqlite3_stmt* stmt; // Statement pointer
        // Prepare the SQL statement
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return {false, {Response::ERROR_FETCH}}; // Return error if preparation fails
        }

        vector<string> records; // Vector to hold fetched records
        // Execute the statement and fetch all records
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* record = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)); // Get the first column's value
            records.push_back(record ? record : ""); // Store record in vector, handle null values
        }

        sqlite3_finalize(stmt); // Finalize the statement

        return {true, records}; // Return all fetched records
    }

    // Update a record in a specified table by its ID
    pair<bool, const char*> update(const string& table, const string& id, unordered_map<string, string>& values) {
        // Fetch the columns of the specified table
        auto columnsResult = this->getTableColumns(table);
        if (!columnsResult.first) {
            return {false, Response::ERROR_TABLE_NOT_FOUND}; // Return error if table is not found
        }

        const vector<string>& columns = columnsResult.second; // Get the list of columns
        string query = "UPDATE " + table + " SET "; // Start building the update query
        bool first = true;

        // Construct the update query dynamically based on provided values
        for (const auto& pair : values) {
            // Check if the column exists in the specified table
            if (std::find(columns.begin(), columns.end(), pair.first) != columns.end()) { // Use std::find to avoid conflict
                if (!first) {
                    query += ", "; // Add comma for subsequent updates
                }
                query += pair.first + " = '" + pair.second + "'"; // Add column-value pair to query
                first = false; // Set first to false after the first update
            }
        }

        query += " WHERE id = '" + id + "';"; // Complete the query with the specified ID

        char* errMsg = nullptr; // Error message pointer
        if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            string error = Response::ERROR_UPDATE; // Handle any update errors
            error += ": ";
            error += errMsg; // Append error message
            sqlite3_free(errMsg); // Free the error message
            return {false, error.c_str()}; // Return failure
        }

        return {true, Response::UPDATE_SUCCESS}; // Return success if update is successful
    }

    // Destroy a record in the specified table by its ID
    pair<bool, const char*> destroy(const string& table, const string& id) {
        string query = "DELETE FROM " + table + " WHERE id = '" + id + "';"; // SQL query to delete the record

        char* errMsg = nullptr; // Error message pointer
        if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            string error = Response::ERROR_DELETE; // Handle any delete errors
            error += ": ";
            error += errMsg; // Append error message
            sqlite3_free(errMsg); // Free the error message
            return {false, error.c_str()}; // Return failure
        }

        return {true, Response::DELETE_SUCCESS}; // Return success if deletion is successful
    }
};

#endif //SQLITEMANAGER_SQLITEMANAGER_H
