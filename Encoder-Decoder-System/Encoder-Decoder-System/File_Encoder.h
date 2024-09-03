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


            File_Encoder(const encoder_type& encoder,
                const std::string& input_file_name,
                const std::string& output_file_name) {
                std::size_t remaining_bytes = wd_codec::fileio::file_size(input_file_name);
                if (remaining_bytes == 0)
                {
                    wd_codec::Logger::log(wd_codec::ERROR, " file_encoder() - Error: input file has ZERO size..");
                    return;
                }
                //open input file to read
                std::ifstream in_stream(input_file_name.c_str(), std::ios::binary);
                if (!in_stream)
                {
                    wd_codec::Logger::log(wd_codec::ERROR, " Error: input file could not be opened");
                    return;
                }
                //open output file to write
                std::ofstream out_stream(output_file_name.c_str(), std::ios::binary);
                if (!out_stream)
                {
                    wd_codec::Logger::log(wd_codec::ERROR, " Error: output file could not be created.");
                    return;
                }

                       std::memset(data_buffer_, 0, sizeof(data_buffer_));
                       std::memset(fec_buffer_, 0, sizeof(fec_buffer_));

                if (remaining_bytes >= data_length) {
                    wd_codec::Logger::log(wd_codec::INFO, "INFO: dividing the file to blocks with size k");
                }
                //divide the file to blocks size k
                while (remaining_bytes >= data_length)
                {
                    //encode each block 
                    process_block(encoder, in_stream, out_stream, data_length);
                    remaining_bytes -= data_length;
                }
                //last block
                if (remaining_bytes > 0)
                {
                    process_block(encoder, in_stream, out_stream, remaining_bytes);
                }
                wd_codec::Logger::log(wd_codec::INFO, " Encode process successed");
                in_stream.close();
                out_stream.close();
            }
        private:

             void process_block(const encoder_type& encoder,
                std::ifstream& in_stream,
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
                }

                  if (!encoder.encode(block_))
                  {
                      wd_codec::Logger::log(wd_codec::ERROR, " file_encoder() - Error during encoding of block!");
                      return;
                  }

                  std::cout << "\nEncode word: [";
                  for (std::size_t i = 0; i < code_length; ++i)
                  {
                      if (i == data_length)
                          std::cout << " ++ ";
                      std::cout << static_cast<char>(block_[i]);
                  }
                  std::cout << "]\n";


                for (std::size_t i = 0; i < fec_length; ++i)
                {
                       fec_buffer_[i] = static_cast<char>(block_.fec(i) & 0xFF);
                }

                out_stream.write(&data_buffer_[0], static_cast<std::streamsize>(read_amount));
                out_stream.write(&fec_buffer_[0], fec_length);

            }
            
            block_type block_;
            char data_buffer_[data_length];
            char fec_buffer_[fec_length];


        };
    }

}



