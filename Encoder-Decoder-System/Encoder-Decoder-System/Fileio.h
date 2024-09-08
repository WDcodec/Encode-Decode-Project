//for fileio- file_size function
#pragma once
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>
#define IMG_TYPE ".BMP" 
#define IMG_TYPE_SIZE 4
// Fileio.cpp

namespace wd_codec {
    namespace fileio {

        inline std::size_t file_size(const std::string& file_name) {
            std::ifstream file(file_name, std::ios::binary | std::ios::ate);
            if (!file) return 0;
            return static_cast<std::size_t>(file.tellg());
        }

        inline bool file_exists(const std::string& file_name)//errors handle in call
        {
            std::ifstream file(file_name.c_str(), std::ios::binary);
            return ((!file) ? false : true);
        }

        inline void write_file(const std::string& file_name, const std::string& buffer) {//todo:handle
            std::ofstream file(file_name, std::ios::binary);
            if (!file) {
                wd_codec::Logger::log(wd_codec::CRITICAL, "Fileio: open file failed!");
                return;
            }
  
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        }

        inline void write_file(const std::string& file_name, const char* buffer, std::size_t buffer_size) {//todo:handle
            std::ofstream file(file_name, std::ios::binary);
            if (!file) {
                wd_codec::Logger::log(wd_codec::CRITICAL, "Fileio: open file failed!");
                return;
            }
            file.write(buffer, static_cast<std::streamsize>(buffer_size));
        }

       inline void create_file(const std::string& file_name, const std::size_t file_size)
        {
            //TODO: reading file from exiting files instead of create new file
            std::string buffer = std::string(file_size, 0x00);

            for (std::size_t i = 0; i < buffer.size() - 1; i += 2)
            {
                //buffer[i] = static_cast<unsigned char>(i & 0xFF);
                buffer[i] = 'o';
                buffer[i + 1] = 'z';
            }

            wd_codec::fileio::write_file(file_name, buffer);
        }



       inline bool convertImageToBinary(const std::string& imageFilePath, const std::string& binaryFilePath)
        {
            std::ifstream image(imageFilePath, std::ios::in | std::ios::binary);
            if (!image)
            {
                std::cerr << "Error opening image file: " << imageFilePath << std::endl;
                return false;
            }

            std::ofstream binary(binaryFilePath, std::ios::out | std::ios::binary);
            if (!binary)
            {
                std::cerr << "Error opening binary output file: " << binaryFilePath << std::endl;
                return false;
            }

            std::vector<char> buffer((std::istreambuf_iterator<char>(image)), std::istreambuf_iterator<char>());
            binary.write(buffer.data(), buffer.size());

            image.close();
            binary.close();

            return true;
        }

       inline bool convertBinaryToImage(const std::string& binaryFilePath, const std::string& imageFilePath)
        {
            std::ifstream binary(binaryFilePath, std::ios::in | std::ios::binary);
            if (!binary)
            {
                std::cerr << "Error opening binary file: " << binaryFilePath << std::endl;
                return false;
            }

            std::ofstream image(imageFilePath, std::ios::out | std::ios::binary);
            if (!image)
            {
                std::cerr << "Error opening image output file: " << imageFilePath << std::endl;
                return false;
            }

            std::vector<char> buffer((std::istreambuf_iterator<char>(binary)), std::istreambuf_iterator<char>());
            //TODO: add a loop that copy from the buffer in chunks of block_size (to not copy the redentucey)
            image.write(buffer.data(), buffer.size());

            binary.close();
            image.close();

            return true;
        }

    }
}
