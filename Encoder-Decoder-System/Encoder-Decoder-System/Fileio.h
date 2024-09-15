//for fileio- file_size function
#pragma once
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>
#include "Logger.h"
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

        inline void write_file(const std::string& file_name, const std::string& buffer) {
            std::ofstream file(file_name, std::ios::binary);
            if (!file) {
                wd_codec::Logger::log(wd_codec::CRITICAL, "Fileio: open file failed!");
                return;
            }
  
            file.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        }

        inline void write_file(const std::string& file_name, const char* buffer, std::size_t buffer_size) {
            std::ofstream file(file_name, std::ios::binary);
            if (!file) {
                wd_codec::Logger::log(wd_codec::CRITICAL, "Fileio: open file failed!");
                return;
            }
            file.write(buffer, static_cast<std::streamsize>(buffer_size));
        }

        inline void create_file(const std::string& file_name, const std::size_t file_size)
        {
            std::string buffer = std::string(file_size, 0x00);

            for (std::size_t i = 0; i < buffer.size() - 1; i += 2)
            {
                //buffer[i] = static_cast<unsigned char>(i & 0xFF);//example 1
                //example 2
                buffer[i] = 'o';
                buffer[i + 1] = 'z';
            }

            wd_codec::fileio::write_file(file_name, buffer);
        }

        inline void converAudioToBinary(const std::string& audioFilename, const std::string& binaryFilename) {
            // Open the Opus file
            wd_codec::Logger::log(wd_codec::INFO, "convet audio to binary");
            std::ifstream audioFile(audioFilename, std::ios::binary);
            if (!audioFile) {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening Opus file.");
                return;
            }

            // Read the Opus file into a buffer
            std::vector<char> buffer((std::istreambuf_iterator<char>(audioFile)), std::istreambuf_iterator<char>());

            // Close the Opus file
            audioFile.close();

            // Open the binary file
            std::ofstream binaryFile(binaryFilename, std::ios::binary);
            if (!binaryFile) {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening binary file.");
                return;
            }

            // Write the buffer to the binary file
            binaryFile.write(buffer.data(), buffer.size());

            // Close the binary file
            binaryFile.close();
        }
        inline void convert_binary_to_audio(const std::string& binaryFilename, const std::string& audioFilename) {
            // Open the binary file
            wd_codec::Logger::log(wd_codec::INFO, "convet binary to audio ");

            std::ifstream binaryFile(binaryFilename, std::ios::binary);
            if (!binaryFile) {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening binary file.");
                return;
            }

            // Read the binary file into a buffer
            std::vector<char> buffer((std::istreambuf_iterator<char>(binaryFile)), std::istreambuf_iterator<char>());

            // Close the binary file
            binaryFile.close();

            // Open the Opus file
            std::ofstream audioFile(audioFilename, std::ios::binary);
            if (!audioFile) {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening Opus file.");
                return;
            }

            // Write the buffer to the Opus file
            audioFile.write(buffer.data(), buffer.size());

            // Close the Opus file
            audioFile.close();
        }

        inline bool convert_image_to_binary(const std::string& imageFilePath, const std::string& binaryFilePath)
        {
            wd_codec::Logger::log(wd_codec::INFO, "convet image to binary ");

            std::ifstream image(imageFilePath, std::ios::in | std::ios::binary);
            if (!image)
            {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening image file: ");
                return false;
            }

            std::ofstream binary(binaryFilePath, std::ios::out | std::ios::binary);
            if (!binary)
            {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening binary output file: ");
                return false;
            }

            std::vector<char> buffer((std::istreambuf_iterator<char>(image)), std::istreambuf_iterator<char>());
            binary.write(buffer.data(), buffer.size());

            image.close();
            binary.close();

            return true;
        }

        template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
       inline bool convert_binary_to_image(const std::string& binaryFilePath, const std::string& imageFilePath)
        {
           wd_codec::Logger::log(wd_codec::INFO, "convet binary to image ");

            std::ifstream binary(binaryFilePath, std::ios::in | std::ios::binary);
            if (!binary)
            {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening binary file: ");
                std::cerr << binaryFilePath << std::endl;
                return false;
            }

            std::ofstream image(imageFilePath, std::ios::out | std::ios::binary);
            if (!image)
            {
                wd_codec::Logger::log(wd_codec::ERROR, "Error opening image output file: ");
                return false;
            }

            std::vector<char> buffer((std::istreambuf_iterator<char>(binary)), std::istreambuf_iterator<char>());
            //TODO: add a loop that copy from the buffer in chunks of block_size (to not copy the redentucey)
            image.write(buffer.data(), buffer.size());

            binary.close();
            image.close();

            return true;
        }
       //todo efrat
        //template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
        //inline bool convertBinaryToImage(const std::string& binaryFilePath, const std::string& imageFilePath) {
        //    std::ifstream binary(binaryFilePath, std::ios::in | std::ios::binary);
        //    if (!binary) {
        //        std::cerr << "Error opening binary file: " << binaryFilePath << std::endl;
        //        return false;
        //    }

        //    std::ofstream image(imageFilePath, std::ios::out | std::ios::binary);
        //    if (!image) {
        //        std::cerr << "Error opening image output file: " << imageFilePath << std::endl;
        //        return false;
        //    }

        //    const std::size_t chunk_size = code_length;
        //    const std::size_t write_size = data_length;

        //    std::vector<char> buffer(chunk_size);

        //    // Read and process the binary file in chunks
        //    while (binary.read(buffer.data(), chunk_size) || binary.gcount() > 0) {
        //        std::size_t bytes_read = static_cast<std::size_t>(binary.gcount());

        //        // Write only the first data_length bytes or bytes_read if it's less than data_length
        //        std::size_t bytes_to_write = std::min(write_size, bytes_read);
        //        image.write(buffer.data(), bytes_to_write);

        //        // If more bytes were read than needed, skip the excess (fec_length)
        //        if (bytes_read > write_size) {
        //            binary.seekg(bytes_read - write_size, std::ios::cur);
        //        }

        //        // If at the end of file, ensure last incomplete chunk is handled
        //        if (binary.eof()) {
        //            break;
        //        }
        //    }

        //    binary.close();
        //    image.close();

        //    return true;
        //}




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

