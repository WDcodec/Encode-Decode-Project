// Encoder-Decoder-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <sstream>
#include <ctime>
#include "Logger.h";
#include "Field.h";
#include "Polynomial.h"
#include "Encoder.h";
#include "Generator_polynomial.h"
#include "Fileio.h"
#include "File_Encoder.h"
#include "Error_Injection.h"
#include "Decoder.h"
#include "File_Decoder.h"
void create_file(const std::string& file_name, const std::size_t file_size)
{
    //TODO: reading file from exiting files instead of create new file
    std::string buffer = std::string(file_size, 0x00);

    for (std::size_t i = 0; i < buffer.size() - 1; i += 2)
    {
        //buffer[i] = static_cast<unsigned char>(i & 0xFF);
        buffer[i] = 'o';
        buffer[i + 1] = 'z';
    }

    wd_codec::fileio::write_file(file_name, buffer);
}

int main()
{
    /*time_t now = time(0);
    tm timeinfo;
    localtime_s(&timeinfo, &now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::string time = timestamp;
    wd_codec::Logger::init("logfile"+time+".txt");*/
    wd_codec::Logger::init("logfile.txt");

    /* Finite Field Parameters */
    const std::size_t field_descriptor = 8;
    const std::size_t generator_polynomial_index = 120;
    const std::size_t generator_polynomial_root_count = 32;

    /* Reed Solomon Code Parameters */
    const std::size_t code_length = 255;
    const std::size_t fec_length = 32;
    const std::size_t data_length = code_length - fec_length;

    const std::size_t stack_size = 255;

    const std::string input_file_name = "input.dat";
    const std::string rsencoded_output_file_name = "output.rsenc";
    const std::string deinterleaved_output_file_name = "output.deintr";

    const std::string rsdecoded_file_name = "output.rsdec";
    const std::size_t gen_poly_index = 120;

    /* Instantiate Finite Field and Generator Polynomials */
    const wd_codec::galois::Field field(field_descriptor,
               wd_codec::galois::primitive_polynomial_size06,
               wd_codec::galois::primitive_polynomial06);
    wd_codec::galois::Polynomial generator_polynomial(field);

    //Generate G(X)
    if (!wd_codec::create_root_generator_polynomial(field,
        generator_polynomial_index,
        generator_polynomial_root_count,
        generator_polynomial)) {
        //TODO: handle error.
        std::cout << "not good";
        return 1;
    }
    wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", generator_polynomial);

    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;

    const encoder_t encoder(field, generator_polynomial);
    const decoder_t decoder(field, gen_poly_index);

    create_file(input_file_name, data_length * stack_size - 3);

    //wd_codec::reed_solomon::File_Encoder<code_length, fec_length>file1();
    //file1.
    wd_codec::reed_solomon::File_Encoder<code_length, fec_length> file_encoder(encoder);

    // Call the encode function
    if (!file_encoder.encode( input_file_name, rsencoded_output_file_name)) {
        std::cout << "Encoding failed." << std::endl;
        return 1;
    }

    wd_codec::error_injection::inject_random_errors<code_length, fec_length>(rsencoded_output_file_name);
    wd_codec::reed_solomon::File_Decoder<code_length, fec_length>
       file_decoder(decoder);
    if (!file_decoder.decode(rsencoded_output_file_name,rsdecoded_file_name)) {
        std::cout << "Encoding failed." << std::endl;
        return 1;
    }
    wd_codec::Logger::close();
}

