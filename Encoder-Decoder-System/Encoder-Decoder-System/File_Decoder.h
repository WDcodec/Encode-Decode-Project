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
            std::size_t getCurrentBlockIndex() const {
                return current_block_index_;
            }
            typedef Decoder<code_length, fec_length> decoder_type;
            typedef typename decoder_type::block_type block_type;

            bool decode_image(const std::string& input_file_name,
                const std::string& output_file_name) {
                bool result = decode(input_file_name, output_file_name);
                const std::string imageFilePath = "binary_image_corrected.bmp";
                if (wd_codec::fileio::convert_binary_to_image<code_length, fec_length>(output_file_name, imageFilePath)) {
#ifdef DEBUG
                    wd_codec::Logger::log(wd_codec::INFO, "Binary to image conversion complete!");
#endif // DEBUG
                }
                else {
                    wd_codec::Logger::log(wd_codec::ERROR, "Binary to image conversion failed!");
                }
                return result;
            }

            bool decode_audio(const std::string& input_file_name,
                const std::string& output_file_name) {
                bool result = decode(input_file_name, output_file_name);
                const std::string imageFilePath = "binary_audio_corrected.opus";
                wd_codec::fileio::convert_binary_to_audio(output_file_name, imageFilePath);
                return result;
            }

            File_Decoder(const decoder_type& decoder) : decoder(decoder), current_block_index_(0) {};

            bool decode(const std::string& input_file_name,
                const std::string& output_file_name)
            {

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
                wd_codec::Logger::log(wd_codec::INFO, "File Decoder: decodeing file ");
                std::string message;
                    while (remaining_bytes >= code_length)
                    {
                        message = "Decoding block number "+ std::to_string(current_block_index_);
                        wd_codec::Logger::log(INFO, message);

                        if (!process_complete_block(in_stream, out_stream)) {
                            wd_codec::errors_block_locations += " ";
                            wd_codec::errors_block_locations += std::to_string(current_block_index_);
                            wd_codec::errors_block_locations += ",";
                            wd_codec::num_uncorrected_blocks ++;
                            failed_decode = false;
                        }                          
                        remaining_bytes -= code_length;
                        current_block_index_++;
                    }
                     
                    if (remaining_bytes > 0)
                    {
                        if (!process_partial_block(in_stream, out_stream, remaining_bytes))
                            failed_decode = false;
                    }
                    if(!wd_codec::errors_block_locations.empty())
                        wd_codec::errors_block_locations.pop_back();
                in_stream.close();
                out_stream.close();
                #ifdef DEBUG
                wd_codec::Logger::log(wd_codec::INFO, "File Decoder: Decoder succeeded" );
                #endif // DEBUG
               // wd_codec::Logger::logErrorsNumber();
                wd_codec::num_blocks = current_block_index_;
                return failed_decode;
            }
            bool get_is_residue_handled() {
                return is_residue_handled;
             }

            std::size_t get_current_block_index_() {
                return current_block_index_;
            }

        private:

            inline bool process_complete_block(
                std::ifstream& in_stream,
                std::ofstream& out_stream)
            {
                in_stream.read(&buffer_[0], static_cast<std::streamsize>(code_length));
                copy<char, code_length, fec_length>(buffer_, code_length, block_);
                bool decoder_succeed = true;
                    if (!decoder.decode(block_))
                    {
                        wd_codec::Logger::log(wd_codec::ERROR, "File Decoder::process_complete_block(): Error during decoding of block " + std::to_string(current_block_index_) );
                        wd_codec::global_errors_detected += block_.errors_detected;
                        decoder_succeed = false;
                    }
                    else {
                        wd_codec::global_errors_detected += block_.errors_detected;
                        wd_codec::global_errors_corrected += block_.errors_corrected;
                    }
                     
                    for (std::size_t i = 0; i < data_length; ++i)
                    {
                        buffer_[i] = static_cast<char>(block_[i]);
                    }    
                    out_stream.write(&buffer_[0], static_cast<std::streamsize>(data_length));
                    return decoder_succeed;
                }

                inline bool process_partial_block(
                    std::ifstream& in_stream,
                    std::ofstream& out_stream,
                    const std::size_t& read_amount)
                {
                    if (read_amount <= fec_length)
                    {
                        wd_codec::Logger::log(wd_codec::CRITICAL, "File Decoder::process_partial_block(): Error during decoding of block" /*<< current_block_index_ << "!"*/);
                        return false;
                    }
                 bool decoder_succeed = true;
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
                    is_residue_handled = true;
                }

                for (std::size_t i = 0; i < fec_length; ++i)
                {
                    block_.fec(i) = static_cast<typename block_type::symbol_type>(buffer_[(read_amount - fec_length) + i]);
                }

                    if (!decoder.decode(block_))
                    {
                        wd_codec::Logger::log(wd_codec::ERROR, "File Decoder::process_partial_block(): Error during decoding of block" /*<< current_block_index_<<"!"*/);
                        wd_codec::global_errors_detected += block_.errors_detected;
                        decoder_succeed = false;
                    }
                    else{
                    wd_codec::global_errors_detected += block_.errors_detected;
                    wd_codec::global_errors_corrected += block_.errors_corrected;}
                for (std::size_t i = 0; i < (read_amount - fec_length); ++i)
                {
                    buffer_[i] = static_cast<char>(block_.data[i]);
                }

                    out_stream.write(&buffer_[0], static_cast<std::streamsize>(read_amount - fec_length));
                    return decoder_succeed;
                }

                const decoder_type& decoder;
                block_type block_;
                int current_block_index_;
                char buffer_[code_length];  
                bool is_residue_handled = false;
                bool failed_decode = true;
		};

    }
}