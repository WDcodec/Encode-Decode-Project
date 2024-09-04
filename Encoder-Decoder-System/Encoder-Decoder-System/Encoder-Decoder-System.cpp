////// Encoder-Decoder-System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//////
////
////#include <sstream>
////#include <ctime>
////#include "Logger.h";
////#include "Field.h";
////#include "Polynomial.h"
////#include "Encoder.h";
////#include "Generator_polynomial.h"
////#include "Fileio.h"
////#include "File_Encoder.h"
////#include "Error_Injection.h"
////#include "Decoder.h"
////#include "File_Decoder.h"
////void create_file(const std::string& file_name, const std::size_t file_size)
////{
////    //TODO: reading file from exiting files instead of create new file
////    std::string buffer = std::string(file_size, 0x00);
////
////    for (std::size_t i = 0; i < buffer.size() - 1; i += 2)
////    {
////        //buffer[i] = static_cast<unsigned char>(i & 0xFF);
////        buffer[i] = 'o';
////        buffer[i + 1] = 'z';
////    }
////
////    wd_codec::fileio::write_file(file_name, buffer);
////}
////
////int main()
////{
////    /*time_t now = time(0);
////    tm timeinfo;
////    localtime_s(&timeinfo, &now);
////    char timestamp[20];
////    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
////    std::string time = timestamp;
////    wd_codec::Logger::init("logfile"+time+".txt");*/
////    wd_codec::Logger::init("logfile.txt");
////
////    /* Finite Field Parameters */
////    const std::size_t field_descriptor = 8;
////    const std::size_t generator_polynomial_index = 120;
////    const std::size_t generator_polynomial_root_count = 32;
////
////    /* Reed Solomon Code Parameters */
////    const std::size_t code_length = 255;
////    const std::size_t fec_length = 32;
////    const std::size_t data_length = code_length - fec_length;
////
////    const std::size_t stack_size = 255;
////
////    const std::string input_file_name = "input.dat";
////    const std::string rsencoded_output_file_name = "output.rsenc";
////    const std::string deinterleaved_output_file_name = "output.deintr";
////
////    const std::string rsdecoded_file_name = "output.rsdec";
////    const std::size_t gen_poly_index = 120;
////
////    /* Instantiate Finite Field and Generator Polynomials */
////    const wd_codec::galois::Field field(field_descriptor,
////               wd_codec::galois::primitive_polynomial_size06,
////               wd_codec::galois::primitive_polynomial06);
////    wd_codec::galois::Polynomial generator_polynomial(field);
////
////    //Generate G(X)
////    if (!wd_codec::create_root_generator_polynomial(field,
////        generator_polynomial_index,
////        generator_polynomial_root_count,
////        generator_polynomial)) {
////        //TODO: handle error.
////        std::cout << "not good";
////        return 1;
////    }
////    wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", generator_polynomial);
////
////    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
////    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;
////
////    const encoder_t encoder(field, generator_polynomial);
////    const decoder_t decoder(field, gen_poly_index);
////
////    create_file(input_file_name, data_length * stack_size - 3);
////
////    wd_codec::reed_solomon::File_Encoder<code_length, fec_length>
////        (
////            encoder,
////            input_file_name,//the original file
////            rsencoded_output_file_name//the encoded file
////        );
////
////   wd_codec::error_injection::inject_random_errors<code_length, fec_length>(rsencoded_output_file_name);
////   wd_codec::reed_solomon::File_Decoder<code_length, fec_length>
////       (
////           decoder,
////           rsencoded_output_file_name,
////           rsdecoded_file_name
////       );
////
////    wd_codec::Logger::close();
////}
//#include <sstream>
//#include <ctime>
//#include <fstream>
//#include <vector>
//#include "Logger.h"
//#include "Field.h"
//#include "Polynomial.h"
//#include "Encoder.h"
//#include "Generator_polynomial.h"
//#include "Fileio.h"
//#include "File_Encoder.h"
//#include "Error_Injection.h"
//#include "Decoder.h"
//#include "File_Decoder.h"
//
//void create_file(const std::string& file_name, const std::size_t file_size)
//{
//    //TODO: reading file from existing files instead of creating a new file
//    std::string buffer = std::string(file_size, 0x00);
//
//    for (std::size_t i = 0; i < buffer.size() - 1; i += 2)
//    {
//        buffer[i] = 'o';
//        buffer[i + 1] = 'z';
//    }
//
//    wd_codec::fileio::write_file(file_name, buffer);
//}
//
//bool convertImageToBinary(const std::string& imageFilePath, const std::string& binaryFilePath)
//{
//    std::ifstream image(imageFilePath, std::ios::in | std::ios::binary);
//    if (!image)
//    {
//        std::cerr << "Error opening image file: " << imageFilePath << std::endl;
//        return false;
//    }
//
//    std::ofstream binary(binaryFilePath, std::ios::out | std::ios::binary);
//    if (!binary)
//    {
//        std::cerr << "Error opening binary output file: " << binaryFilePath << std::endl;
//        return false;
//    }
//
//    std::vector<char> buffer((std::istreambuf_iterator<char>(image)), std::istreambuf_iterator<char>());
//    binary.write(buffer.data(), buffer.size());
//
//    image.close();
//    binary.close();
//
//    return true;
//}
//
//bool convertBinaryToImage(const std::string& binaryFilePath, const std::string& imageFilePath)
//{
//    std::ifstream binary(binaryFilePath, std::ios::in | std::ios::binary);
//    if (!binary)
//    {
//        std::cerr << "Error opening binary file: " << binaryFilePath << std::endl;
//        return false;
//    }
//
//    std::ofstream image(imageFilePath, std::ios::out | std::ios::binary);
//    if (!image)
//    {
//        std::cerr << "Error opening image output file: " << imageFilePath << std::endl;
//        return false;
//    }
//
//    std::vector<char> buffer((std::istreambuf_iterator<char>(binary)), std::istreambuf_iterator<char>());
//    //TODO: add a loop that copy from the buffer in chunks of block_size (to not copy the redentucey)
//    image.write(buffer.data(), buffer.size());
//
//    binary.close();
//    image.close();
//
//    return true;
//}
//
//int main()
//{
//    wd_codec::Logger::init("logfile.txt");
//
//    /* Finite Field Parameters */
//    const std::size_t field_descriptor = 8;
//    const std::size_t generator_polynomial_index = 120;
//    const std::size_t generator_polynomial_root_count = 32;
//
//    /* Reed Solomon Code Parameters */
//    const std::size_t code_length = 255;
//    const std::size_t fec_length = 32;
//    const std::size_t data_length = code_length - fec_length;
//    const std::size_t stack_size = 255;
//
//    // Converting image to binary for encoding
//    const std::string imageFilePath = "picture.bmp";
//    const std::string binaryFilePath = "binary_image_data.bin";
//    if (convertImageToBinary(imageFilePath, binaryFilePath)) {
//        std::cout << "Image to binary conversion complete!" << std::endl;
//    }
//    else {
//        std::cerr << "Image to binary conversion failed!" << std::endl;
//    }
//    const std::string input_file_name = binaryFilePath;
//    const std::string rsencoded_output_file_name = "output.bin";
//    const std::string rsdecoded_file_name = "output.rsdec";
//
//    /* Instantiate Finite Field and Generator Polynomials */
//    const wd_codec::galois::Field field(field_descriptor,
//        wd_codec::galois::primitive_polynomial_size06,
//        wd_codec::galois::primitive_polynomial06);
//    wd_codec::galois::Polynomial generator_polynomial(field);
//
//    // Generate G(X)
//    if (!wd_codec::create_root_generator_polynomial(field,
//        generator_polynomial_index,
//        generator_polynomial_root_count,
//        generator_polynomial)) {
//        std::cerr << "Error generating G(X)" << std::endl;
//        return 1;
//    }
//
//    wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", generator_polynomial);
//
//    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
//    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;
//
//    const encoder_t encoder(field, generator_polynomial);
//    const decoder_t decoder(field, generator_polynomial_index);
//
//
//    //create_file(input_file_name, data_length * stack_size - 3);
//
//    wd_codec::reed_solomon::File_Encoder<code_length, fec_length>(
//        encoder,
//        input_file_name,
//        rsencoded_output_file_name
//    );
//
//    wd_codec::error_injection::inject_random_errors<code_length, fec_length>(rsencoded_output_file_name);
//
//
//    // Decoding and converting back to image
//    if (convertBinaryToImage(rsencoded_output_file_name, "restored_picture_corrupted.bmp")) {
//        std::cout << "Binary to image conversion complete!" << std::endl;
//    }
//    else {
//        std::cerr << "Binary to image conversion failed!" << std::endl;
//    }
//
//    wd_codec::reed_solomon::File_Decoder<code_length, fec_length>
//        (
//            decoder,
//            rsencoded_output_file_name,
//            rsdecoded_file_name
//        );
//
//
//    // Decoding and converting back to image
//    if (convertBinaryToImage(rsdecoded_file_name, "restored_picture_correct.bmp")) {
//        std::cout << "Binary to image conversion complete!" << std::endl;
//    }
//    else {
//        std::cerr << "Binary to image conversion failed!" << std::endl;
//    }
//
//    wd_codec::Logger::close();
//
//    return 0;
//}
//#include <gtest/gtest.h>
//
//int main(int argc, char** argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}