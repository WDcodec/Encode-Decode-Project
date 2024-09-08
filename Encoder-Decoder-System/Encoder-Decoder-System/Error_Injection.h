#pragma once
#include "Fileio.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <string>
#include <set>
namespace wd_codec {
	namespace error_injection {

        template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
        inline void inject_random_errors_for_image(const std::string& file_name) {
            inject_random_errors<code_length, fec_length>(file_name,0, code_length);
            const std::string imageFilePath = "binary_image_corrupted.bmp";
            wd_codec::fileio::convertBinaryToImage(file_name, imageFilePath);
        }

        template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
        inline void inject_random_errors(const std::string& file_name, int errors_number = 0, std::size_t start = 0){        {
            if (!wd_codec::fileio::file_exists(file_name))
            {
                std::cout << "Error: " << file_name << " does not exist!" << std::endl;
                return;
            }

            std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
            if (!file)
            {
                std::cout << "Error: Unable to open file for reading!" << std::endl;
                return;
            }

            // Get the file size
            std::streamsize file_size = wd_codec::fileio::file_size(file_name);

            std::vector<char> buffer(data_length);

            // Initialize random number generator:
            std::random_device rd;                                         // Seed generator
            std::mt19937 gen(rd());                                        // Random number engine
            std::uniform_int_distribution<> byte_dis(0, data_length - 1);  // Uniform distribution [0, data_length]

            // Process the file in chunks
            for (std::streamoff offset = start; offset < file_size; offset += code_length)
            {
                std::streamsize current_chunk_size = (data_length< file_size - offset)? data_length: (file_size - offset);
                file.seekg(offset);
                file.read(buffer.data(), current_chunk_size);

                // Inject random errors
                std::set<std::size_t> error_indices;
                int counter_errors = 0;
                while (counter_errors < fec_length / 2 + errors_number)
                {
                    std::size_t random_index = byte_dis(gen);
                    // Valid condition
                    if (random_index < current_chunk_size)
                    {
                        error_indices.insert(random_index);
                        counter_errors++;
                    }
                }

                for (std::size_t index : error_indices)
                {
                    //TODO: change to operator ~
                    buffer[index] = char(buffer[index] + 2); // Corrupt the byte
                    //std::cout << "index= " << index << " buffer[index]= " << (char)buffer[index] << " ~buffer[index]= " << (char)(buffer[index]-2) << "\n";
                }

                // Write the corrupted chunk back to the file
                std::ofstream file_out(file_name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
                if (!file_out)
                {
                    std::cout << "Error: Unable to open file for writing!" << std::endl;
                    return;
                }

                file_out.seekp(offset);
                file_out.write(buffer.data(), current_chunk_size);
                file_out.close();
            }

            file.close();
        }


        inline void corrupt_file_with_burst_errors(const std::string& file_name,
            const long& start_position,
            const long& burst_length)
        {
            if (!wd_codec::fileio::file_exists(file_name))
            {
                std::cout << "corrupt_file() - Error: " << file_name << " does not exist!" << std::endl;
                return;
            }

            if (static_cast<std::size_t>(start_position + burst_length) >= wd_codec::fileio::file_size(file_name))
            {
                std::cout << "corrupt_file() - Error: Burst error out of bounds." << std::endl;
                return;
            }

            std::vector<char> data(burst_length);

            std::ifstream ifile(file_name.c_str(), std::ios::in | std::ios::binary);

            if (!ifile)
            {
                return;
            }

            ifile.seekg(start_position, std::ios_base::beg);
            ifile.read(&data[0], burst_length);
            ifile.close();

            for (long i = 0; i < burst_length; ++i)
            {
                data[i] = ~data[i];
            }

            std::ofstream ofile(file_name.c_str(), std::ios::in | std::ios::out | std::ios::binary);

            if (!ofile)
            {
                return;
            }

            ofile.seekp(start_position, std::ios_base::beg);
            ofile.write(&data[0], burst_length);
            ofile.close();
        }
	}
}