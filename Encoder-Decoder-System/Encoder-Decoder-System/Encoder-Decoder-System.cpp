
// Encoder-Decoder-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "setup_utilities.h"
int main()
{
    wd_codec::setup();
    const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
    const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

    wd_codec::file_encoder_t file_encoder(encoder);
    wd_codec::file_decoder_t file_decoder(decoder);

    std::cout << "Enter 1 for primitive input-\n 2 for file input\n 3 for image file";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        //TODO: handle string input;
        std::string message = "An expert is someone who knows more and more about less and "
            "less until they know absolutely everything about nothing";
        message.resize(wd_codec::code_length, 0x00);
        if (!encoder.encode(wd_codec::block, message))
        {
            std::cout << "Error - Critical encoding failure! ";
            return 1;
        }
        else {
            std::cout << "\nEncode word: [";
            for (std::size_t i = 0; i < wd_codec::code_length; ++i)
            {
                std::cout << static_cast<char>(wd_codec::block[i]);
            }
            std::cout << "]\n";
        }

    }
    if(choice == 2) {        //TODO: handle the files names will be input from the client
        const std::string input_file_name = "input.dat";
        const std::string rsencoded_output_file_name = "output.rsenc";
        const std::string deinterleaved_output_file_name = "output.deintr";
        const std::string rsdecoded_file_name = "output.rsdec";
        //wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
        // Call the encode function
        wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", wd_codec::generator_polynomial);
        if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
            std::cout << "Encoding failed." << std::endl;
            return 1;
        }
        wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name);
    }
    if (choice == 3)
    {
        //TODO: handle the files names will be input from the client
        const std::string input_file_name = "picture.bmp";
        const std::string rsencoded_output_file_name = "output.rsenc";
        const std::string deinterleaved_output_file_name = "output.deintr";
        const std::string rsdecoded_file_name = "output.rsdec";
        //wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
        // Call the encode function
        wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", wd_codec::generator_polynomial);
        if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
            std::cout << "Encoding failed." << std::endl;
            return 1;
        }
        wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name);
        const std::string imageFilePath = "binary_image_data.bmp";
        wd_codec::fileio::convertBinaryToImage(rsencoded_output_file_name, imageFilePath);
    }

    wd_codec::close();

}


