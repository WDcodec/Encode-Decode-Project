
#pragma once
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "Field.h"
#include "Polynomial.h"
#include "Encoder.h"
#include "Logger.h"
#include "Fileio.h"
#include "Block.h"


namespace wd_codec {

    namespace reed_solomon {
        template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
        class File_Encoder
        {

        public:
            typedef Encoder<code_length, fec_length> encoder_type;
            typedef typename encoder_type::block_type block_type;

            File_Encoder(const encoder_type& encoder) :encoder(encoder), blocks_number(0){
                static_assert(data_length > 0, "data_length must be greater than 0");
                if (data_length <= 0) {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "Encoder FAILED: data_length is non-positive");
                    throw std::invalid_argument("data_length must be greater than 0");
                }
            };

            bool encode_image(const std::string& input_file_name,
                const std::string& output_file_name) {
                const std::string binaryFilePath = "binary_image_data.bin";
                fileio::convert_image_to_binary(input_file_name, binaryFilePath);
                return encode(binaryFilePath, output_file_name);
            }

            bool encode_audio(const std::string& input_file_name,
                const std::string& output_file_name) {
                const std::string binaryFilePath = "binary_audio_data.bin";
                fileio::converAudioToBinary(input_file_name, binaryFilePath);
                return encode(binaryFilePath, output_file_name);
            }

            bool encode(const std::string& input_file_name,
                const std::string& output_file_name)
            {
                wd_codec::Logger::log(wd_codec::INFO, "INFO: encoding");

                std::size_t remaining_bytes = wd_codec::fileio::file_size(input_file_name);
                if (remaining_bytes == 0)
                {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "File Encoder: input file has ZERO size.");
                    return false;
                }
                //open input file to read
                std::ifstream in_stream(input_file_name.c_str(), std::ios::binary);
                if (!in_stream)
                {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "File Encoder: input file could not be opened.");
                    return false;
                }
                //open output file to write
                std::ofstream out_stream(output_file_name.c_str(), std::ios::binary);
                if (!out_stream)
                {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "File Encoder: output file could not be created.");
                    return false;
                }

                std::memset(data_buffer_, 0, sizeof(data_buffer_));
                std::memset(fec_buffer_, 0, sizeof(fec_buffer_));

                if (remaining_bytes >= data_length) {
                #ifdef _DEBUG
                    wd_codec::Logger::log(wd_codec::INFO, "INFO: dividing the file to blocks with size k");
                 #endif // DEBUG
                }
                //divide the file to blocks size k
                while (remaining_bytes >= data_length)
                {
                    //encode each block
                    process_block(in_stream, out_stream, data_length);
                    inc_blocks_number();
                    remaining_bytes -= data_length;
                }
                //last block
                if (remaining_bytes > 0)
                {
                    process_block(in_stream, out_stream, remaining_bytes);
                    
                }
                #ifdef _DEBUG
                wd_codec::Logger::log(wd_codec::INFO, "File Encoder: Encode process successed"); 
                #endif // DEBUG
              
                in_stream.close();
                out_stream.close();

                return true;
            }

            std::size_t get_blocks_number() {
                return blocks_number;
            }

            void set_blocks_number(std::size_t number) {
                blocks_number = number;
            }

            void inc_blocks_number() {
                blocks_number++;
            }
            bool get_is_residue_handled() {
                return is_residue_handled;
            }
        private:

            bool process_block(std::ifstream& in_stream,
                std::ofstream& out_stream,
                const std::size_t& read_amount) {
                in_stream.read(&data_buffer_[0], static_cast<std::streamsize>(read_amount));
                //copy k bytes from the buffer to current block
                for (std::size_t i = 0; i < read_amount; ++i)
                {
                    block_.data[i] = (data_buffer_[i] & 0xFF);
                }

                // padding zeros if needed
                if (read_amount < data_length)
                {
                    for (std::size_t i = read_amount; i < data_length; ++i)
                    {
                        block_.data[i] = 0x00;
                    }
                    is_residue_handled = true;
                }

                if (!encoder.encode(block_))
                {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "File Encoder: Error during encoding block.");
                    return false;
                }
                //print each block
             /*   std::cout << "\nEncode word: [";
                for (std::size_t i = 0; i < code_length; ++i)
                {
                    if (i == data_length)
                        std::cout << " ++ ";
                    std::cout << static_cast<char>(block_[i]);
                }
                std::cout << "]\n";*/


                for (std::size_t i = 0; i < fec_length; ++i)
                {
                    fec_buffer_[i] = static_cast<char>(block_.fec(i) & 0xFF);
                }

                out_stream.write(&data_buffer_[0], static_cast<std::streamsize>(read_amount));
                out_stream.write(&fec_buffer_[0], fec_length);

                return true;
            }

            const encoder_type& encoder;
            block_type block_;
            std::size_t blocks_number;
            char data_buffer_[data_length];

            char fec_buffer_[fec_length];
            bool is_residue_handled = false;

        };
    }

}

