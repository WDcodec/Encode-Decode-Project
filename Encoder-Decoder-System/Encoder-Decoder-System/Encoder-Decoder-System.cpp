
// Encoder-Decoder-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "setup_utilities.h"
int main()
{//todo : insted of if encode return 1 just call encode function
    //FLSE OR TRUE FOR THE DECIDE OF THE DEVELOPER
    wd_codec::setup();
    const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
    const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

    wd_codec::file_encoder_t file_encoder(encoder);
    wd_codec::file_decoder_t file_decoder(decoder);

    std::cout << "Enter 1 for primitive input-\n 2 for file input\n 3 for image file\n 4  for audio file\n";
    int choice;
    std::cin >> choice;
    wd_codec::Logger::increaseCoverage();
    if (choice == 1) {
        //TODO: handle string input;
        std::string message = "Tamar Ayala and Efrat developed the best encoder deocder system\n, they worked hard, but at the end it was worth it";
        message.resize(wd_codec::code_length, 0x00);
        if (!encoder.encode(wd_codec::block, message))
        {

            return 1;
        }
        else {
            wd_codec::Logger::increaseCoverage();
            std::cout << "\nEncode word: [";
            for (std::size_t i = 0; i < wd_codec::code_length; ++i)
            {
                std::cout << static_cast<char>(wd_codec::block[i]);
            }
            std::cout << "]\n";
        }
        if (!decoder.decode(wd_codec::block)) {
            wd_codec::Logger::increaseCoverage();

            return 1;
        }
        else {
            std::cout << "\Decode word: [";
            for (std::size_t i = 0; i < wd_codec::data_length; ++i)
            {
                std::cout << static_cast<char>(wd_codec::block[i]);
            }
            std::cout << "]\n";
        }


    }
    else if (choice == 2) {        //TODO: handle the files names will be input from the client
        const std::string input_file_name = "input.dat";
        const std::string rsencoded_output_file_name = "output.rsenc";
        const std::string rsdecoded_file_name = "output.rsdec";
        wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
        // Call the encode function
        if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
            return 1;
        }
        wd_codec::Logger::increaseCoverage();
        wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name);
        wd_codec::Logger::increaseCoverage();
        if (!file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name)) {
            wd_codec::Logger::log(wd_codec::INFO, "File Decoder: Decoder failed " );
            wd_codec::close();
            return 1;
        }
        else {
            std::cout << "Decoding succeed." << std::endl;
        }
        std::cout << "num blocks " << file_decoder.get_current_block_index_();
    }
    else if (choice == 3)
    {
        //TODO: handle the files names will be input from the client
        const std::string input_file_name = "picture.bmp";
        const std::string rsencoded_output_file_name = "output.rsenc";
        const std::string rsdecoded_file_name = "output.rsdec";
        // Call the encode function
        const std::string binaryFilePath = "binary_image_data.bin";
        wd_codec::fileio::convert_image_to_binary(input_file_name, binaryFilePath);
        wd_codec::error_injection::inject_random_errors_for_image<wd_codec::code_length, wd_codec::fec_length>(binaryFilePath, "binary_image_corrupted.bmp");
        if (!file_encoder.encode_image(input_file_name, rsencoded_output_file_name)) {
            std::cout << "Encoding failed." << std::endl;
            wd_codec::close();
            return 1;
        }
        wd_codec::Logger::increaseCoverage();
        wd_codec::error_injection::inject_random_errors_for_image<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name, "binary_image_encoded.bmp");
        wd_codec::Logger::increaseCoverage();
        if (!file_decoder.decode_image(rsencoded_output_file_name, rsdecoded_file_name)) {
            std::cout << "Decoding failed." << std::endl;
            wd_codec::close();
            return 1;
        }
        else {
            std::cout << "Decoding succeed." << std::endl;
        }
        std::cout << "num blocks " << file_decoder.get_current_block_index_();
        wd_codec::Logger::increaseCoverage();


    }

    else if (choice == 4)
    {
        //TODO: handle the files names will be input from the client
        const std::string input_file_name = "audio.opus";
        const std::string rsencoded_output_file_name = "output.rsenc";
        const std::string rsdecoded_file_name = "output.rsdec";
        // Call the encode function
        const std::string binaryFilePath = "binary_audio_data.bin";
        wd_codec::fileio::converAudioToBinary(input_file_name, binaryFilePath);
        wd_codec::error_injection::inject_random_errors_for_audio<wd_codec::code_length, wd_codec::fec_length>(binaryFilePath, "binary_audio_corrupted.opus");
        if (!file_encoder.encode_audio(input_file_name, rsencoded_output_file_name)) {
            std::cout << "Encoding failed." << std::endl;
            wd_codec::close();
            return 1;
        }
        wd_codec::Logger::increaseCoverage();
        wd_codec::error_injection::inject_random_errors_for_audio<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name, "binary_audio_encoded.opus");
        wd_codec::Logger::increaseCoverage();
        if (!file_decoder.decode_audio(rsencoded_output_file_name, rsdecoded_file_name)) {
            std::cout << "Decoding failed." << std::endl;
            wd_codec::close();
            return 1;
        }
        else {
            std::cout << "Decoding succeed." << std::endl;
        }
        std::cout << "num blocks " << file_decoder.get_current_block_index_();
        wd_codec::Logger::increaseCoverage();
    }
    else {
        wd_codec::Logger::log(wd_codec::ERROR, "your choice is invalid!");

    }

    wd_codec::close();

}
//
//
