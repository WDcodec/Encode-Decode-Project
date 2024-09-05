// Encoder-Decoder-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "setup_utilities.h"

int main()
{
    wd_codec::setup();

    std::cout << "Enter 1 for primitive input-\n 2 for file input\n 3 for image file";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        //TODO: handle string input;
    }
    else {
        //TODO: handle the files names will be input from the client
        const std::string input_file_name = "input.dat";
        const std::string rsencoded_output_file_name = "output.rsenc";
        const std::string deinterleaved_output_file_name = "output.deintr";
        const std::string rsdecoded_file_name = "output.rsdec";

        // Call the encode function
        if (!wd_codec::file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
            std::cout << "Encoding failed." << std::endl;
            return 1;
        }
        wd_codec::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
        wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name);
    }

   wd_codec::close();

}

