#pragma once
class Logger
{
};

#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace wd_codec {
    // Enum to represent log levels 
    enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

    class Logger {
    public:

        // Initializes the logger, Opens the log file in append mode 
        static void init(const std::string& filename) {
            if (!logFile.is_open()) {
                //TODO: check how is better, keeping the logs from previous running or clean it each start of running
                logFile.open(filename, std::ios::out | std::ios::trunc);
                //without deleteing the logs from previous running:
                //logFile.open(filename, std::ios::out);
                if (!logFile.is_open()) {
                    std::cerr << "Error opening log file." << std::endl;
                }
            }
        }

        // Closes the log file
        static void close() {
            if (logFile.is_open()) {
                logFile.close();
            }
        }

        // Logs a message with a given log level 
        static void log(LogLevel level, const std::string& message)
        {
            // Get current timestamp 
            time_t now = time(0);
            tm timeinfo;

            // Use localtime_s or localtime_r based on the platform
#ifdef _WIN32
            localtime_s(&timeinfo, &now);
#else
            localtime_r(&now, &timeinfo);
#endif

            char timestamp[20];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);

            // Create log entry 
            std::ostringstream logEntry;
            logEntry << "[" << timestamp << "] "
                << levelToString(level) << ": " << message
                << std::endl;

            // Output to console 
            std::cout << logEntry.str();

            // Output to log file 
            if (logFile.is_open()) {
                logFile << logEntry.str();
                logFile.flush(); // Ensure immediate write to file 
            }
        }

    private:
        static std::ofstream logFile; // File stream for the log file 

        // Converts log level to a string for output 
        static std::string levelToString(LogLevel level)
        {
            switch (level) {
            case DEBUG:
                return "DEBUG";
            case INFO:
                return "INFO";
            case WARNING:
                return "WARNING";
            case ERROR:
                return "ERROR";
            case CRITICAL:
                return "CRITICAL";
            default:
                return "UNKNOWN";
            }
        }
    };

}