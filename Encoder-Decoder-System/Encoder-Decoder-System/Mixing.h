#pragma once
#include <fstream>    // for file streams (ifstream, ofstream)
#include <vector>     // for std::vector
#include <stdexcept>  // for std::runtime_error
#include <iostream>   // for std::cerr, std::streampos
#include "Block.h"    // For your Block class and Galois field definitions

namespace wd_codec {
    //template <std::size_t code_length, std::size_t fec_length, std::size_t data_length>
    //bool transpose_mix(const std::string& encoded_file_name,const std::string& mixing_file_name,std::size_t num_blocks, std::size_t file_size) {
    //    
    //    std::ifstream in_stream(encoded_file_name.c_str(), std::ios::binary);
    //    if (!in_stream)
    //    {
    //        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: input file could not be opened.");
    //        return false;
    //    }

    //    std::ofstream out_stream(encoded_file_name.c_str(), std::ios::binary);
    //    if (!out_stream)
    //    {
    //        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: output file could not be created.");
    //        return false;
    //    }
    //    std::string buffer = std::string(file_size, 0x00);
    //    // Read blocks into 2D array
    //    in_stream.read(&buffer, file_size);
    //    if (!in_stream) {
    //            throw std::runtime_error("Error reading from input file.");
    //    }
    //    

    //    // Write transposed data to output
    //    for (int block = 0; block < num_blocks; ++block) {
    //        for (std::size_t j = 0; j < file_size; j += data_length) {
    //            out_stream.write(&buffer[block + j], 1);
    //        }
    //    }
    //    return true;
    //}
    template <std::size_t code_length, std::size_t fec_length, std::size_t data_length>
    bool transpose_mix(const std::string& encoded_file_name, const std::string& mixing_file_name, std::size_t num_blocks, std::size_t file_size) {

        // Open input file in binary mode
        std::ifstream in_stream(encoded_file_name.c_str(), std::ios::binary);
        if (!in_stream) {
            wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: input file could not be opened.");
            return false;
        }

        // Open output file in binary mode
        std::ofstream out_stream(mixing_file_name.c_str(), std::ios::binary);
        if (!out_stream) {
            wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: output file could not be created.");
            return false;
        }

        // Create a buffer to hold the input file contents
        std::string buffer(file_size, 0x00);

        // Read file contents into buffer
        in_stream.read(&buffer[0], file_size);  // Use &buffer[0] to get a modifiable pointer
        if (!in_stream) {
            throw std::runtime_error("Error reading from input file.");
        }

        // Write transposed data to output file
        for (std::size_t block = 0; block < code_length; ++block) {
            for (std::size_t j = block ; j < file_size; j += code_length) {
                std::cout << buffer[j]<<" "<<"block: "<<block<<"\n";
                out_stream.write(&buffer[j], 1);
            }
        }

        return true;
    }



    template <std::size_t code_length, std::size_t fec_length, std::size_t data_length>
    bool transpose_remix2(const std::string& encoded_file_name, const std::string& mixing_file_name, std::size_t num_blocks, std::size_t file_size) {

        // Open input file in binary mode
        std::ifstream in_stream(encoded_file_name.c_str(), std::ios::binary);
        if (!in_stream) {
            wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: input file could not be opened.");
            return false;
        }

        // Open output file in binary mode
        std::ofstream out_stream(mixing_file_name.c_str(), std::ios::binary);
        if (!out_stream) {
            wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: output file could not be created.");
            return false;
        }

        // Create a buffer to hold the input file contents
        std::string buffer(file_size, 0x00);

        // Read file contents into buffer
        in_stream.read(&buffer[0], file_size);  // Use &buffer[0] to get a modifiable pointer
        if (!in_stream) {
            throw std::runtime_error("Error reading from input file.");
        }

        // Write transposed data to output file
        for (std::size_t block = 0; block < num_blocks; ++block) {
            for (std::size_t j = block; j < file_size; j += num_blocks) {
                out_stream.write(&buffer[j], 1);
            }
        }

        return true;
    }
    //template <std::size_t code_length, std::size_t fec_length, std::size_t data_length>
    //bool transpose_remix(const std::string& encoded_file_name, const std::string& mixing_file_name, std::size_t num_blocks, std::size_t file_size) {

    //    // Open input file in binary mode
    //    std::ifstream in_stream(encoded_file_name.c_str(), std::ios::binary);
    //    if (!in_stream) {
    //        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: input file could not be opened.");
    //        return false;
    //    }

    //    // Open output file in binary mode
    //    std::ofstream out_stream(mixing_file_name.c_str(), std::ios::binary);
    //    if (!out_stream) {
    //        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: output file could not be created.");
    //        return false;
    //    }

    //    // Create a buffer to hold the input file contents
    //    std::string buffer(file_size, 0x00);

    //    // Read file contents into buffer
    //    in_stream.read(&buffer[0], file_size);  // Use &buffer[0] to get a modifiable pointer
    //    if (!in_stream) {
    //        throw std::runtime_error("Error reading from input file.");
    //    }

    //    // Write transposed data to output file
    //    for (std::size_t block = 0; block < num_blocks; ++block) {
    //        for (std::size_t j = block; j < file_size; j += data_length) {
    //            out_stream.write(&buffer[j], 1);
    //        }
    //    }

    //    return true;
    //}

    template <std::size_t code_length, std::size_t fec_length, std::size_t data_length>
    bool transpose_remix(const std::string& transposed_file_name, const std::string& remixed_file_name, std::size_t num_blocks, std::size_t file_size) {
        // Open transposed (mixed) file in binary mode
        std::ifstream in_stream(transposed_file_name.c_str(), std::ios::binary);
        if (!in_stream) {
            wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: transposed file could not be opened.");
            return false;
        }

        // Open remixed file (output) in binary mode
        std::ofstream out_stream(remixed_file_name.c_str(), std::ios::binary);
        if (!out_stream) {
            wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: remixed output file could not be created.");
            return false;
        }

        // Create a buffer to hold the input (transposed) file contents
        std::string buffer(file_size, 0x00);

        // Read transposed file contents into the buffer
        in_stream.read(&buffer[0], file_size);
        if (!in_stream) {
            throw std::runtime_error("Error reading from transposed file.");
        }

        // Create a buffer to store the remixed (original order) data
        std::string remixed_buffer(file_size, 0x00);

        // Reverse the transpose (restore original order)
        for (std::size_t block = 0; block < num_blocks; ++block) {
            for (std::size_t j = block; j < file_size; j += code_length) {
                // We use j to write the bytes back into their original block positions
                remixed_buffer[j] = buffer[block + (j / code_length) * code_length];
            }
        }

        // Write the remixed buffer (restored original data) back to the output file
        out_stream.write(remixed_buffer.c_str(), file_size);
        if (!out_stream) {
            throw std::runtime_error("Error writing to remixed file.");
        }

        return true;
    }

//    template <std::size_t code_length, std::size_t fec_length, std::size_t data_length>
//    void transpose_unmix(std::ifstream& in_stream, std::ofstream& out_stream, std::size_t num_blocks, std::size_t num_columns) {
//        std::vector<Block<code_length, fec_length, data_length>> blocks(num_blocks, Block<code_length, fec_length, data_length>());
//
//        // Read transposed data into 2D array
//        for (std::size_t col = 0; col < num_columns; ++col) {
//            for (std::size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
//                in_stream.read(reinterpret_cast<char*>(&blocks[block_idx].data[col]), sizeof(galois::field_symbol));
//                if (!in_stream) {
//                    throw std::runtime_error("Error reading from input file.");
//                }
//            }
//        }
//
//        // Write blocks row by row to output
//        for (const auto& block : blocks) {
//            out_stream.write(reinterpret_cast<const char*>(block.data), code_length);
//            if (!out_stream) {
//                throw std::runtime_error("Error writing to output file.");
//            }
//        }
//    }
//
}