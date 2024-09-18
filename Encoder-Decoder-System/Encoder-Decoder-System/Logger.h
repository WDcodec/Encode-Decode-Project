
#pragma once

#include "report.h"
#include "Polynomial.h"

#define POSTFIX ".txt"
#define NUM_STAGES 7
namespace wd_codec {

    // Enum to represent log levels 
    enum LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };
    static bool test_mode_empty_file = false;
    static std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    static std::chrono::time_point<std::chrono::high_resolution_clock> duration;

    class Logger {
    public:

        // Initializes the logger, opens the log file in append mode 
        static void init() {
            char timestamp[20];
            start_timer();
            create_timestamp(timestamp);
            coverage = 0;
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
                    wd_codec::Logger::log(wd_codec::ERROR, "Error opening log file.");
                }
            }
        }
        
        // Call this function at the beginning of the program to start the timer
        static void start_timer() {
            start_time = std::chrono::high_resolution_clock::now();
        }

        // Call this function at the end of the program to log the elapsed time and other metrics
        static void log_report_metrics_summary(long long& duration) {
            auto end_time = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
            std::ostringstream elapsed_time_message;
            wd_codec::Logger::log("-REPORT- ");
            elapsed_time_message << "* Elapsed time: " << duration << " milliseconds";
            wd_codec::Logger::log_errors_number();
            wd_codec::Logger::log( "* Coverage system: " + std::to_string((coverage * 100) / NUM_STAGES) + "% done.");
            wd_codec::Logger::log( elapsed_time_message.str());
        }

        // Function that log the report of the errors after the process
        static void log_errors_number() {
            // Create log entry 
            std::ostringstream logEntry;
            std::size_t num_corrected_blocks = num_blocks - num_uncorrected_blocks;
            logEntry << "* Number of error that detected: " << wd_codec::global_errors_detected
                << "\n"
                << "* Number of error that corected: " << wd_codec::global_errors_corrected
                << "\n"
                << "* Number of blocks: " << num_blocks
                << "\n"
                << "* Number of corrected blocks: " << num_corrected_blocks
                << "\n"
                << "* Number of uncorrected blocks: " << num_uncorrected_blocks
                << "\n"
                << "  The uncorrected blocks are:" << errors_block_locations
                << "\n"
                << "* Success rate: " <<  (((double)(num_corrected_blocks)/ num_blocks) * 100)
                << "%" 
                << std::endl;


            // Output to console 
            std::cout << logEntry.str();

            // Output to log file 
            if (logFile.is_open()) {
                logFile << logEntry.str();
                logFile.flush(); // Ensure immediate write to file 
            }

        }

    // Overloading of log function:
        static void log(LogLevel level, const std::string& message, galois::Polynomial& poly) {
            std::string newMessage = message + poly.convert_to_string();
            wd_codec::Logger::log(wd_codec::INFO, newMessage);

        }

        static void log(LogLevel level, const std::string& message)
        {
            char timestamp[20];
            create_timestamp(timestamp);

            // Create log entry 
            std::ostringstream logEntry;
            logEntry << "[" << timestamp << "] "
                << levelToString(level) << ": " << message
                << std::endl;
//#ifdef _DEBUG||_RELEASE//todo for run tests 
            // Output to console 
            std::cout << logEntry.str();
//#endif
            // Output to log file 
            if (logFile.is_open()) {
                logFile << logEntry.str();
                logFile.flush(); // Ensure immediate write to file 
            }
        }
        static void log(const std::string& message)
        {
            // Create log entry 
            std::ostringstream logEntry;
            logEntry <<  message
                << std::endl;
            // Output to console 
            std::cout << logEntry.str();
            // Output to log file 
            if (logFile.is_open()) {
                logFile << logEntry.str();
                logFile.flush(); // Ensure immediate write to file 
            }
        }

        // Closes the log file
        static void close() {
            char timestamp[20];
            create_timestamp(timestamp);
            log_report_metrics_summary(duration);
            log_update_total_metrics_report_for_all_runnings(duration, timestamp);
            if (logFile.is_open()) {
                logFile.close();
            }
        }
        static void increaseCoverage() {
            if (coverage < NUM_STAGES) {
                coverage++;
            }
        }
    private:
        static std::ofstream logFile; // File stream for the log file 
        static int coverage;
        static long long duration;
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