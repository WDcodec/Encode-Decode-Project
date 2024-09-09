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

       //template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
       //inline bool convertBinaryToImage(const std::string& binaryFilePath, const std::string& imageFilePath)
       //{
       //    std::ifstream binary(binaryFilePath, std::ios::in | std::ios::binary);
       //    if (!binary)
       //    {
       //        std::cerr << "Error opening binary file: " << binaryFilePath << std::endl;
       //        return false;
       //    }

       //    std::ofstream image(imageFilePath, std::ios::out | std::ios::binary);
       //    if (!image)
       //    {
       //        std::cerr << "Error opening image output file: " << imageFilePath << std::endl;
       //        return false;
       //    }

       //    const std::size_t chunk_size = code_length;
       //    const std::size_t write_size = data_length;

       //    std::vector<char> buffer(chunk_size);

       //    while (binary.read(buffer.data(), chunk_size))
       //    {
       //        // Number of bytes actually read
       //        std::size_t bytes_read = binary.gcount();

       //        // Write only the first data_length bytes or bytes_read if it's less than data_length
       //        std::size_t bytes_to_write = std::min(write_size, bytes_read);
       //        image.write(buffer.data(), bytes_to_write);

       //        // Skip remaining bytes in the current chunk
       //        if (bytes_read > write_size)
       //        {
       //            binary.ignore(bytes_read - write_size);
       //        }
       //    }

       //    // Handle the case where the last chunk is less than chunk_size
       //    if (binary.gcount() > 0)
       //    {
       //        std::size_t bytes_read = binary.gcount();
       //        std::size_t bytes_to_write = std::min(write_size, bytes_read);
       //        image.write(buffer.data(), bytes_to_write);
       //    }

       //    binary.close();
       //    image.close();

       //    return true;
       //}

    }
}

