#pragma once
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <algorithm>
namespace wd_codec {
	static std::size_t global_errors_detected = 0;
	static std::size_t global_errors_corrected = 0;
    static std::size_t num_uncorrected_blocks = 0;
    static std::size_t num_blocks = 0;
    static std::string errors_block_locations = " ";

    enum LogEntryType {
        TOTAL_EXECUTIONS,
        TOTAL_RUNTIME,
        TOTAL_ERRORS_DETECTED,
        TOTAL_ERRORS_CORRECTED,
        MIN_SUCCESS_RATE,
        MAX_SUCCESS_RATE,
        MIN_RUNTIME,
        MAX_RUNTIME,
        MIN_SUCCESS_ERRORS_CORRECTED,
        MAX_SUCCESS_ERRORS_CORRECTED,
        UNKNOWN
    };

    // Function to map log entry label to enum value


    // Function to map log entry label to enum value
    inline static LogEntryType getLogEntryType(const std::string& line) {
        if (line.find("Total Executions") != std::string::npos) return TOTAL_EXECUTIONS;
        if (line.find("Total Runtime") != std::string::npos) return TOTAL_RUNTIME;
        if (line.find("Total Errors Detected") != std::string::npos) return TOTAL_ERRORS_DETECTED;
        if (line.find("Total Errors Corrected") != std::string::npos) return TOTAL_ERRORS_CORRECTED;
        if (line.find("Min Success Rate") != std::string::npos) return MIN_SUCCESS_RATE;
        if (line.find("Max Success Rate") != std::string::npos) return MAX_SUCCESS_RATE;
        if (line.find("Min Runtime") != std::string::npos) return MIN_RUNTIME;
        if (line.find("Max Runtime") != std::string::npos) return MAX_RUNTIME;
        if (line.find("Min Success Errors Corrected") != std::string::npos) return MIN_SUCCESS_ERRORS_CORRECTED;
        if (line.find("Max Success Errors Corrected") != std::string::npos) return MAX_SUCCESS_ERRORS_CORRECTED;

        return UNKNOWN;
    }
inline static void log_update_total_metrics_report_for_all_runnings(long long duration, const char* timestamp) {
    std::ifstream inputFile("summary_log.txt");

    // Initialize the metrics
    int totalExecutions = 0;
    double totalRuntime = 0;
    int totalErrorsCorrected = 0;
    int totalErrorsDetected = 0;
    int totalNumBlocks = 0;
    int totalNumCorrectedBlocks = 0;
    double averageRuntime = 0;
    double totalSuccessRate = 0;
    double minRuntime = std::numeric_limits<double>::max();
    double maxRuntime = 0;

    // Use std::string instead of raw pointers for these
    std::string minRuntimeRunning;
    std::string maxRuntimeRunning;
    int minSuccessErrorsCorrected = std::numeric_limits<int>::max();
    int maxSuccessErrorsCorrected = 0;
    std::string minSuccessErrorsCorrectedRunning;
    std::string maxSuccessErrorsCorrectedRunning;
    double minSuccessRate = std::numeric_limits<double>::max();
    double maxSuccessRate = 0;
    std::string minSuccessRateRunning;
    std::string maxSuccessRateRunning;

    // Read the existing log file if it exists
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            LogEntryType entryType = getLogEntryType(line);
            std::istringstream iss(line);
            std::string label;

            switch (entryType) {
            case TOTAL_EXECUTIONS:
                iss >> label >> label >> totalExecutions;
                break;
            case TOTAL_RUNTIME:
                iss >> label >> label >> label >> totalRuntime;
                break;
            case TOTAL_ERRORS_DETECTED:
                iss >> label >> label >> label >> totalErrorsDetected;
                break;
            case TOTAL_ERRORS_CORRECTED:
                iss >> label >> label >> label >> totalErrorsCorrected;
                break;
            case MIN_SUCCESS_RATE:
                iss >> label >> label >> label >> minSuccessRate;
                break;
            case MAX_SUCCESS_RATE:
                iss >> label >> label >> label >> maxSuccessRate;
                break;
            case MIN_RUNTIME:
                iss >> label >> label >> label >> minRuntime;
                break;
            case MAX_RUNTIME:
                iss >> label >> label >> label >> maxRuntime;
                break;
            case MIN_SUCCESS_ERRORS_CORRECTED:
                iss >> label >> label >> label >> label >> minSuccessErrorsCorrected;
                break;
            case MAX_SUCCESS_ERRORS_CORRECTED:
                iss >> label >> label >> label >> label >> maxSuccessErrorsCorrected;
                break;
            default:
                break;
            }
        }
        inputFile.close();
    }

    // Update the metrics
    totalExecutions++;
    totalRuntime += duration;
    totalErrorsDetected += global_errors_detected;  // Assuming global variables
    totalErrorsCorrected += global_errors_corrected;  // Assuming global variables
    double successRate = (num_blocks > 0) ? (num_blocks - num_uncorrected_blocks) / (double)num_blocks : 0;  // Avoid division by zero
    totalSuccessRate = successRate * 100;
    averageRuntime = totalRuntime / totalExecutions;
    totalNumBlocks += num_blocks;
    totalNumCorrectedBlocks += (num_blocks - num_uncorrected_blocks);

    // Use std::min and std::max for updating metrics
    minRuntime = std::min(minRuntime, (double)duration);
    minRuntimeRunning = (minRuntime == duration) ? timestamp : minRuntimeRunning;
    maxRuntime = std::max(maxRuntime, (double)duration);
    maxRuntimeRunning = (maxRuntime == duration) ? timestamp : maxRuntimeRunning;

    minSuccessErrorsCorrected = std::min<int>(minSuccessErrorsCorrected, static_cast<int>(global_errors_corrected));
    minSuccessErrorsCorrectedRunning = (minSuccessErrorsCorrected == global_errors_corrected) ? timestamp : minSuccessErrorsCorrectedRunning;

    maxSuccessErrorsCorrected = std::max<int>(maxSuccessErrorsCorrected, static_cast<int>(global_errors_corrected));
    maxSuccessErrorsCorrectedRunning = (maxSuccessErrorsCorrected == global_errors_corrected) ? timestamp : maxSuccessErrorsCorrectedRunning;

    minSuccessRate = std::min(minSuccessRate, successRate);
    minSuccessRateRunning = (minSuccessRate == successRate) ? timestamp : minSuccessRateRunning;

    maxSuccessRate = std::max(maxSuccessRate, successRate);
    maxSuccessRateRunning = (maxSuccessRate == successRate) ? timestamp : maxSuccessRateRunning;

    // Write the updated data back to the file
    std::ofstream outputFile("summary_log.txt");
    outputFile << "Total Executions: " << totalExecutions << "\n";
    outputFile << "Total Runtime (milliseconds): " << totalRuntime << "\n";
    outputFile << "Total Errors Detected: " << totalErrorsDetected << "\n";
    outputFile << "Total Errors Corrected: " << totalErrorsCorrected << "\n";
    outputFile << "Total Number Of Blocks: " << totalNumBlocks << "\n";
    outputFile << "Total Number Of Corrected Blocks: " << totalNumCorrectedBlocks << "\n";
    outputFile << "Total Success Rate: " << totalSuccessRate << "%\n";
    outputFile << "Min Success Rate: " << minSuccessRate << " On Running: " << minSuccessRateRunning << "\n";
    outputFile << "Max Success Rate: " << maxSuccessRate << " On Running: " << maxSuccessRateRunning << "\n";
    outputFile << "Min Runtime (milliseconds): " << minRuntime << " On Running: " << minRuntimeRunning << "\n";
    outputFile << "Max Runtime (milliseconds): " << maxRuntime << " On Running: " << maxRuntimeRunning << "\n";
    outputFile << "Min Success Errors Corrected: " << minSuccessErrorsCorrected << " On Running: " << minSuccessErrorsCorrectedRunning << "\n";
    outputFile << "Max Success Errors Corrected: " << maxSuccessErrorsCorrected << " On Running: " << maxSuccessErrorsCorrectedRunning << "\n";
    outputFile << "Average Runtime (milliseconds): " << averageRuntime << "\n";
    outputFile << "Last Run: " << timestamp << "\n";
    outputFile.close();
}

    // Function that update the matrics in summary_log file, with the current matrics
 
    // Creates a timestamp string in the format "YYYY-MM-DD HH:MM:SS"
    inline static void create_timestamp(char* timestamp) {
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


}