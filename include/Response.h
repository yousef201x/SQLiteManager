#ifndef SQLITEMANAGER_RESPONSE_H
#define SQLITEMANAGER_RESPONSE_H

class Response {
public:
    static constexpr const char* INSERT_SUCCESS = "Insert successful";
    static constexpr const char* CONNECTION_OPEN_SUCCESS = "Database connection opened successfully";
    static constexpr const char* CONNECTION_CLOSE_SUCCESS = "Database connection closed successfully";

    static constexpr const char* ERROR_OPEN_DB = "Error opening database";
    static constexpr const char* ERROR_CLOSE_DB = "Error closing database";
    static constexpr const char* ERROR_FETCH_COLUMNS = "Error while fetching column names";
    static constexpr const char* ERROR_TABLE_NOT_FOUND = "Table not found or failed to fetch columns";
    static constexpr const char* ERROR_INSERT = "Error while inserting";
    static constexpr const char* ERROR_PREPARE_STATEMENT = "Failed to prepare SQL statement";
    static constexpr const char* ERROR_FINALIZE_STATEMENT = "Failed to finalize SQL statement";

    static constexpr const char* ERROR_UPDATE = "Error: Update failed.";
    static constexpr const char* UPDATE_SUCCESS = "Update successful.";

    static constexpr const char* ERROR_DELETE = "Error: Delete failed.";
    static constexpr const char* DELETE_SUCCESS = "Delete successful.";

    static constexpr const char* ERROR_FETCH = "Error fetching data from the database.";
    static constexpr const char* ERROR_RECORD_NOT_FOUND = "Record not found.";
};


#endif //SQLITEMANAGER_RESPONSE_H
