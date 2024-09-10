#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Polynomial.h"
#define POSTFIX ".txt"
namespace wd_codec {
    // Enum to represent log levels 
    enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

    class Logger {
    public:

        // Creates a timestamp string in the format "YYYY-MM-DD HH:MM:SS"
        static void create_timestamp(char* timestamp) {
            // Get current timestamp 
            time_t now = time(0);
            tm timeinfo;

            // Use localtime_s or localtime_r based on the platform
#ifdef _WIN32
            localtime_s(&timeinfo, &now);
#else
            localtime_r(&now, &timeinfo);
#endif

            // Format the timestamp
            strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", &timeinfo);
        }

        // Initializes the logger, opens the log file in append mode 
        static void init() {
            char timestamp[20];
            create_timestamp(timestamp);

            // Convert C-style timestamp to std::string for concatenation
            std::string filename = std::string("logfile_") + timestamp + POSTFIX;

            // Replace spaces and colons in the filename to avoid file system issues
            for (char& c : filename) {
                if (c == ' ' || c == ':') {
                    c = '_';  // Replace spaces and colons with underscores
                }
            }

            if (!logFile.is_open()) {
                // Open the log file in append mode
                logFile.open(filename, std::ios::out | std::ios::trunc);
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

        static void log(LogLevel level, const std::string& message, galois::Polynomial& poly) {
            std::string newMessage = message + poly.convert_to_string();
            wd_codec::Logger::log(wd_codec::INFO, newMessage);

        }

        // Logs a message with a given log level 
        static void log(LogLevel level, const std::string& message)
        {
           
            char timestamp[20];
            create_timestamp(timestamp);

            // Create log entry 
            std::ostringstream logEntry;
            logEntry << "[" << timestamp << "] "
                << level_to_string(level) << ": " << message
                << std::endl;

            // Output to console 
            std::cout << logEntry.str();

            // Output to log file 
            if (logFile.is_open()) {
                logFile << logEntry.str();
                logFile.flush(); // Ensure immediate write to file 
            }
            // Handle critical errors by exiting the program
            if (level == CRITICAL) {
                handle_critical_error(message);
            }
        }
        // Handles critical errors by logging and terminating the program
        static void handle_critical_error(const std::string& message) {
            std::cerr << "Terminating program..." << std::endl;
            logFile << "Terminating program..." << std::endl;
            logFile.flush(); // Ensure the message is written to the file before exiting
            close(); // Close the log file properly before exiting
            std::exit(1); // Graceful exit with a critical error code
        }

    private:
        static std::ofstream logFile; // File stream for the log file 

        // Converts log level to a string for output 
        static std::string level_to_string(LogLevel level)
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