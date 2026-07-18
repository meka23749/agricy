#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

struct Measurement;

class Database
{
public:
    explicit Database(const std::string& path);
    ~Database();

    bool initialize();
    bool insertMeasurement(const Measurement& measurement);

    Measurement getLatestMeasurement();
    std::vector<Measurement> getMeasurements();

private:
    sqlite3* db_;
};