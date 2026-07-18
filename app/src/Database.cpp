#include "Database.hpp"
#include "SensorSimulator.hpp"

Database::Database(const std::string& path)
{
    sqlite3_open(path.c_str(), &db_);
}

Database::~Database()
{
    sqlite3_close(db_);
}

bool Database::initialize()
{
    const char* sql =
        "CREATE TABLE IF NOT EXISTS measurements ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "temperature REAL NOT NULL,"
        "humidity REAL NOT NULL"
        ");";

    return sqlite3_exec(db_, sql, nullptr, nullptr, nullptr) == SQLITE_OK;
}

bool Database::insertMeasurement(const Measurement& m)
{
    const char* sql =
        "INSERT INTO measurements "
        "(temperature, humidity) "
        "VALUES (?, ?);";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);

    sqlite3_bind_double(stmt, 1, m.temperature);
    sqlite3_bind_double(stmt, 2, m.humidity);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);

    return success;
}

Measurement Database::getLatestMeasurement()
{
    Measurement measurement{};

    const char* sql =
        "SELECT temperature, humidity "
        "FROM measurements "
        "ORDER BY id DESC "
        "LIMIT 1;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            measurement.temperature =
                sqlite3_column_double(stmt, 0);

            measurement.humidity =
                sqlite3_column_double(stmt, 1);
        }
    }

    sqlite3_finalize(stmt);

    return measurement;
}

std::vector<Measurement> Database::getMeasurements()
{
    std::vector<Measurement> measurements;

    const char* sql =
        "SELECT temperature, humidity "
        "FROM measurements "
        "ORDER BY id;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Measurement measurement;

            measurement.temperature =
                sqlite3_column_double(stmt, 0);

            measurement.humidity =
                sqlite3_column_double(stmt, 1);

            measurements.push_back(measurement);
        }
    }

    sqlite3_finalize(stmt);

    return measurements;
}