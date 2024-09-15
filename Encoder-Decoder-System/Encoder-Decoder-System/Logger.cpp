#include "Logger.h"
namespace wd_codec {
    std::ofstream Logger::logFile; // Definition
    //std::chrono::time_point<std::chrono::high_resolution_clock> Logger::start_time;
    long long wd_codec::Logger::duration = 0;
    int wd_codec::Logger::coverage ;
}