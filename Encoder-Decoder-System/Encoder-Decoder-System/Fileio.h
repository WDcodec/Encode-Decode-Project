//for fileio- file_size function
#pragma once
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>
// Fileio.cpp

namespace wd_codec {
    namespace fileio {

        inline std::size_t file_size(const std::string& file_name) {
            std::ifstream file(file_name, std::ios::binary | std::ios::ate);
            if (!file) return 0;
            return static_cast<std::size_t>(file.tellg());
        }

        inline bool file_exists(const std::string& file_name)
        {
            std::ifstream file(file_name.c_str(), std::ios::binary);
            return ((!file) ? false : true);
        }

        inline void write_file(const std::string& file_name, const std::string& buffer) {
            std::ofstream file(file_name, std::ios::binary);
            if (!file) return;
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        }

        inline void write_file(const std::string& file_name, const char* buffer, std::size_t buffer_size) {
            std::ofstream file(file_name, std::ios::binary);
            if (!file) return;
            file.write(buffer, static_cast<std::streamsize>(buffer_size));
        }

    }
}
