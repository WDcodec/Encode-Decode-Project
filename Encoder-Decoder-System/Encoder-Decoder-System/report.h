#pragma once
#include <cstddef>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
namespace wd_codec {
	static std::size_t global_errors_detected = 0;
	static std::size_t global_errors_corrected = 0;

    // Function that update the matrics in summary_log file, with the current matrics
    inline static void log_update_total_metrics_report_for_all_runnings(long long duration, const char* timestamp) {

        std::ifstream inputFile("summary_log.txt");
        int totalExecutions = 0;
        double totalRuntime = 0;
        int totalErrorsCorrected = 0;
        int totalErrorsDetected = 0;
        double averageRuntime = 0;
        double totalSuccessRate = 0;

        // Read the existing log file if it exists
        if (inputFile.is_open()) {
            std::string line;
            while (std::getline(inputFile, line)) {
                std::istringstream iss(line);
                std::string label;
                if (line.find("Total Executions") != std::string::npos) {
                    iss >> label >> label >> totalExecutions;
                }
                else if (line.find("Total Runtime") != std::string::npos) {
                    iss >> label >> label >> label >> totalRuntime;
                }
                else if (line.find("Total Errors Detected") != std::string::npos) {
                    iss >> label >> label >> label >> totalErrorsDetected;
                }
                else if (line.find("Total Errors Corrected") != std::string::npos) {
                    iss >> label >> label >> label >> totalErrorsCorrected;
                }
            }
            inputFile.close();
}

        // Update values
        totalExecutions++;
        totalRuntime += duration;
        totalErrorsDetected += global_errors_detected;
        totalErrorsCorrected += global_errors_corrected;
        totalSuccessRate = (totalErrorsCorrected / (double)totalErrorsDetected) * 100;
        averageRuntime = totalRuntime / totalExecutions;

        // Write the updated data back to the file
        std::ofstream outputFile("summary_log.txt");
        outputFile << "Total Executions: " << totalExecutions << "\n";
        outputFile << "Total Runtime (milliseconds): " << totalRuntime << "\n";
        outputFile << "Total Errors Detected: " << totalErrorsDetected << "\n";
        outputFile << "Total Errors Corrected: " << totalErrorsCorrected << "\n";
        outputFile << "Total Success rate: " << totalSuccessRate << "%\n";
        outputFile << "Average Runtime (milliseconds): " << averageRuntime << "\n";
        outputFile << "Last Run: " << timestamp << "\n";
        outputFile.close();
    }


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