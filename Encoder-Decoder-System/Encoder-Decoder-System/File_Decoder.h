#pragma once
#include <iostream>
#include <fstream>
#include "Block.h"
#include "Decoder.h"
#include "Fileio.h"
namespace wd_codec {
    namespace reed_solomon {
        template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
        class File_Decoder
        {
        public:

            typedef Decoder<code_length, fec_length> decoder_type;
            typedef typename decoder_type::block_type block_type;
            bool decode_image(const std::string& input_file_name,
                const std::string& output_file_name) {
                decode(input_file_name, output_file_name);
                const std::string imageFilePath = "binary_image_corrected.bmp";
                wd_codec::fileio::convertBinaryToImage(output_file_name, imageFilePath);
            }
            File_Decoder(const decoder_type& decoder) : decoder(decoder), current_block_index_(0) {};

            bool decode(const std::string& input_file_name,
                const std::string& output_file_name)

            {//todo:add success decode IS NESSECARY?

                std::size_t remaining_bytes = wd_codec::fileio::file_size(input_file_name);

                if (remaining_bytes == 0)
                {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: input file has ZERO size.");
                    return false;
                }

                std::ifstream in_stream(input_file_name.c_str(), std::ios::binary);
                if (!in_stream)
                {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: input file could not be opened.");
                    return false;
                }

                std::ofstream out_stream(output_file_name.c_str(), std::ios::binary);
                if (!out_stream)
                {
                    wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder: output file could not be created.");
                    return false;
                }

                current_block_index_ = 0;

                while (remaining_bytes >= code_length)
                {
                    process_complete_block(in_stream, out_stream);
                    remaining_bytes -= code_length;
                    current_block_index_++;
                }

                if (remaining_bytes > 0)
                {
                    process_partial_block(in_stream, out_stream, remaining_bytes);
                }

                in_stream.close();
                out_stream.close();
                /*      if (input_file_name.length() >= IMG_TYPE_SIZE && input_file_name.substr(input_file_name.length() - IMG_TYPE_SIZE) == IMG_TYPE) {
                          const std::string imageFilePath = "binary_image_data.bmp";
                          fileio::convertBinaryToImage(output_file_name, imageFilePath);
                      }*/
                wd_codec::Logger::log(wd_codec::INFO, "File Decoder: Decoder succeeded" );
                return true;
            }
               

        private:

                inline bool process_complete_block(
                    std::ifstream& in_stream,
                    std::ofstream& out_stream)
                {
                    in_stream.read(&buffer_[0], static_cast<std::streamsize>(code_length));
                    copy<char, code_length, fec_length>(buffer_, code_length, block_);

                    if (!decoder.decode(block_))
                    {
                        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder::process_complete_block(): Error during decoding of block"<< current_block_index_ );
                        return false;
                    }
                    for (std::size_t i = 0; i < data_length; ++i)
                    {
                        buffer_[i] = static_cast<char>(block_[i]);
                    }    
                    out_stream.write(&buffer_[0], static_cast<std::streamsize>(data_length));
                    return true;
                }

                inline bool process_partial_block(
                    std::ifstream& in_stream,
                    std::ofstream& out_stream,
                    const std::size_t& read_amount)
                {
                    if (read_amount <= fec_length)
                    {
                        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder::process_partial_block(): Error during decoding of block" << current_block_index_);
                        return false;
                    }

                in_stream.read(&buffer_[0], static_cast<std::streamsize>(read_amount));

                for (std::size_t i = 0; i < (read_amount - fec_length); ++i)
                {
                    block_.data[i] = static_cast<typename block_type::symbol_type>(buffer_[i]);
                }

                if ((read_amount - fec_length) < data_length)
                {
                    for (std::size_t i = (read_amount - fec_length); i < data_length; ++i)
                    {
                        block_.data[i] = 0;
                    }
                }

                for (std::size_t i = 0; i < fec_length; ++i)
                {
                    block_.fec(i) = static_cast<typename block_type::symbol_type>(buffer_[(read_amount - fec_length) + i]);
                }

                    if (!decoder.decode(block_))
                    {
                        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder::process_partial_block(): Error during decoding of block" << current_block_index_);
                        return false;
                    }

                for (std::size_t i = 0; i < (read_amount - fec_length); ++i)
                {
                    buffer_[i] = static_cast<char>(block_.data[i]);
                }

                    out_stream.write(&buffer_[0], static_cast<std::streamsize>(read_amount - fec_length));
                    return true;
                }
                const decoder_type& decoder;
                block_type block_;
                std::size_t current_block_index_;
                char buffer_[code_length];  
		};

    }
}