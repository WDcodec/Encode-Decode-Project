
#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "Polynomial.h"

#define POSTFIX ".txt"
namespace wd_codec {
    // Enum to represent log levels 
    enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };
    static bool test_mode_empty_file = false;
    static std::chrono::time_point<std::chrono::high_resolution_clock> start_time;

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
            start_timer();
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
        // Call this function at the beginning of the program to start the timer
        static void start_timer() {
            start_time = std::chrono::high_resolution_clock::now();
        }

        // Call this function at the end of the program to log the elapsed time
        static void log_elapsed_time() {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            std::ostringstream elapsed_time_message;
            elapsed_time_message << "Elapsed time: " << duration << " milliseconds";
            wd_codec::Logger::log(INFO, elapsed_time_message.str());
        }
        // Closes the log file
        static void close() {
            log_elapsed_time();

            if (logFile.is_open()) {
                logFile.close();
            }
        }

        static void log(LogLevel level,const std::string& message, galois::Polynomial& poly) {
            std::string newMessage = message + poly.convert_to_string();
            wd_codec::Logger::log(wd_codec::INFO, newMessage);

        }
        static void logErrorsNumber(bool success,int erros_detected,int number) {
            // Create log entry 
            std::ostringstream logEntry;
            logEntry << "Number of error that detected: " << error_detected
                << "\n"
                << "Number of error that corected: " << number
                << std::endl;

            // Output to console 
            std::cout << logEntry.str();

            // Output to log file 
            if (logFile.is_open()) {
                logFile << logEntry.str();
                logFile.flush(); // Ensure immediate write to file 
            }
        }
        // Logs a message with a given log level 
        static void log(LogLevel level, const std::string& message)
        {
           
            char timestamp[20];
            create_timestamp(timestamp);

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