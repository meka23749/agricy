#include "SensorSimulator.hpp"
#include "Database.hpp"
#include <crow.h>

#include <fstream>
#include <sstream>

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    SensorSimulator simulator;
    Database database("agricy.db");

    database.initialize();

    std::thread simulatorThread(
        [&simulator, &database]()
        {
            while (true)
            {
                Measurement measurement =
                    simulator.generate();

                database.insertMeasurement(
                    measurement
                );

                std::cout
                    << "Temp: "
                    << measurement.temperature
                    << " Humidity: "
                    << measurement.humidity
                    << '\n';

                std::this_thread::sleep_for(
                    std::chrono::seconds(5)
                );
            }
        }
    );

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
    {
        std::ifstream file("/agricy/app/web/index.html");;

        if (!file)
            return crow::response(404, "index.html not found");

        std::stringstream buffer;
        buffer << file.rdbuf();

        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/health")
    ([]()
    {
        crow::json::wvalue response;
        response["status"] = "ok";
        return response;
    });

    CROW_ROUTE(app, "/latest")
    ([&database]()
    {
        auto latest =
            database.getLatestMeasurement();

        crow::json::wvalue response;

        response["temperature"] =
            latest.temperature;

        response["humidity"] =
            latest.humidity;

        return response;
    });

    CROW_ROUTE(app, "/measurements")
    ([&database]()
    {
        auto measurements =
            database.getMeasurements();

        crow::json::wvalue result;

        for (size_t i = 0;
             i < measurements.size();
             ++i)
        {
            result[i]["temperature"] =
                measurements[i].temperature;

            result[i]["humidity"] =
                measurements[i].humidity;
        }

        return result;
    });

    CROW_ROUTE(app, "/style.css")
    ([]()
    {
        std::ifstream file("../agricy/app/web/style.css");

        if (!file)
            return crow::response(404);

        std::stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/css");
        return res;
    });

    CROW_ROUTE(app, "/app.js")
    ([]()
    {
        std::ifstream file("../agricy/app/web/app.js");

        if (!file)
            return crow::response(404);

        std::stringstream buffer;
        buffer << file.rdbuf();

        crow::response res(buffer.str());
        res.set_header("Content-Type", "application/javascript");
        return res;
    });

    app.port(8080)
       .multithreaded()
       .run();

    simulatorThread.join();

    return 0;
}