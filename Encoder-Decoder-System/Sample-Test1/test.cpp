//#include "pch.h"
////#include "gtest/gtest.h"
////TEST(TestCaseName, TestName) {
////  EXPECT_EQ(1, 1);
////  EXPECT_TRUE(true);
////}
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include "Fileio.h"
#include "File_Encoder.h"
#include "File_Decoder.h"
#include "Encoder.h"
#include "Decoder.h"
#include "Logger.h"
#include "Field.h"
#include "Polynomial.h"
#include "Generator_polynomial.h"

// Helper function to create an empty file
void createEmptyFile(const std::string& file_name) {
    std::ofstream file(file_name, std::ios::out | std::ios::binary);
    // The file is created empty
}

// Setup and teardown functions for logging
void setupTestEnvironment() {
    wd_codec::Logger::init("test_logfile.txt");
}

void teardownTestEnvironment() {
    wd_codec::Logger::close();
}

// Test case for handling an empty file
TEST(EncoderDecoderTests, HandleEmptyFile) {
    setupTestEnvironment();

    const std::string empty_file_name = "empty_input.dat";
    const std::string rsencoded_output_file_name = "empty_output.rsenc";
    const std::string rsdecoded_file_name = "empty_output.rsdec";

    // Create an empty file
    createEmptyFile(empty_file_name);

    // Define Reed-Solomon parameters
    const std::size_t field_descriptor = 8;
    const std::size_t generator_polynomial_index = 120;
    const std::size_t generator_polynomial_root_count = 32;
    const std::size_t code_length = 255;
    const std::size_t fec_length = 32;
    const std::size_t data_length = code_length - fec_length;

    // Instantiate Finite Field and Generator Polynomials
    const wd_codec::galois::Field field(field_descriptor,
        wd_codec::galois::primitive_polynomial_size06,
        wd_codec::galois::primitive_polynomial06);
    wd_codec::galois::Polynomial generator_polynomial(field);

    // Generate G(X)
    if (!wd_codec::create_root_generator_polynomial(field,
        generator_polynomial_index,
        generator_polynomial_root_count,
        generator_polynomial)) {
        FAIL() << "Error generating G(X)";
    }

    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;

    const encoder_t encoder(field, generator_polynomial);
    const decoder_t decoder(field, generator_polynomial_index);

    // Attempt to perform encoding and decoding on the empty file
    bool encode_success = false;
    bool decode_success = false;

    try {
        // Perform encoding
        wd_codec::reed_solomon::File_Encoder<code_length, fec_length>(
            encoder,
            empty_file_name,
            rsencoded_output_file_name
        );

        // Check if the encoded file is empty
        std::ifstream encoded_file(rsencoded_output_file_name, std::ios::binary);
        encode_success = encoded_file.good() && encoded_file.peek() == std::ifstream::traits_type::eof();

        // Perform decoding
        wd_codec::reed_solomon::File_Decoder<code_length, fec_length>(
            decoder,
            rsencoded_output_file_name,
            rsdecoded_file_name
        );

        // Check if the decoded file is empty
        std::ifstream decoded_file(rsdecoded_file_name, std::ios::binary);
        decode_success = decoded_file.good() && decoded_file.peek() == std::ifstream::traits_type::eof();
    }
    catch (const std::exception& e) {
        // Log or handle the exception
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    // Check if the encode/decode process correctly handled the empty file
    EXPECT_FALSE(encode_success) << "Encoding should fail or handle an empty input file correctly.";
    EXPECT_FALSE(decode_success) << "Decoding should handle an empty file appropriately.";

    teardownTestEnvironment();
}
// 
// 
// 
//#include <gtest/gtest.h>
//#include <string>
//#include <fstream>
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
//// Helper function to compare files
//bool compare_files(const std::string& file1, const std::string& file2) {
//    std::ifstream f1(file1, std::ios::binary | std::ios::ate);
//    std::ifstream f2(file2, std::ios::binary | std::ios::ate);
//
//    if (!f1.good() || !f2.good()) return false;
//
//    auto size1 = f1.tellg();
//    auto size2 = f2.tellg();
//
//    if (size1 != size2) return false;
//
//    f1.seekg(0, std::ios::beg);
//    f2.seekg(0, std::ios::beg);
//
//    std::vector<char> buffer1(size1);
//    std::vector<char> buffer2(size2);
//
//    f1.read(buffer1.data(), size1);
//    f2.read(buffer2.data(), size2);
//
//    return std::equal(buffer1.begin(), buffer1.end(), buffer2.begin());
//}
//void create_file(const std::string& file_name, const std::size_t file_size)
//{
//    //TODO: reading file from exiting files instead of create new file
//    std::string buffer = std::string(file_size, 0x00);
//
//    for (std::size_t i = 0; i < buffer.size() - 1; i += 2)
//    {
//        //buffer[i] = static_cast<unsigned char>(i & 0xFF);
//        buffer[i] = 'o';
//        buffer[i + 1] = 'z';
//    }
//
//    wd_codec::fileio::write_file(file_name, buffer);
//}
//// Google Test Case
//TEST(EncoderDecoderTests, FullEncodeDecodeCycle) {
//    // Initialize the logger
//    wd_codec::Logger::init("logfile.txt");
//
//    // Finite Field and Reed-Solomon Parameters
//    const std::size_t field_descriptor = 8;
//    const std::size_t generator_polynomial_index = 120;
//    const std::size_t generator_polynomial_root_count = 32;
//    const std::size_t code_length = 255;
//    const std::size_t fec_length = 32;
//    const std::size_t data_length = code_length - fec_length;
//    const std::size_t stack_size = 255;
//
//    const std::string input_file_name = "input.dat";
//    const std::string rsencoded_output_file_name = "output.rsenc";
//    const std::string rsdecoded_file_name = "output.rsdec";
//
//    // Create a valid input file
//    create_file(input_file_name, data_length * stack_size - 3);
//
//    // Instantiate Finite Field and Generator Polynomials
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
//        FAIL() << "Error generating G(X)";
//    }
//    wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", generator_polynomial);
//
//    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
//    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;
//
//    const encoder_t encoder(field, generator_polynomial);
//    const decoder_t decoder(field, generator_polynomial_index);
//
//    // Perform encoding
//    wd_codec::reed_solomon::File_Encoder<code_length, fec_length>(
//        encoder,
//        input_file_name,
//        rsencoded_output_file_name
//    );
//
//    // Introduce errors into the encoded file
//    wd_codec::error_injection::inject_random_errors<code_length, fec_length>(rsencoded_output_file_name);
//
//    // Perform decoding
//    wd_codec::reed_solomon::File_Decoder<code_length, fec_length>(
//        decoder,
//        rsencoded_output_file_name,
//        rsdecoded_file_name
//    );
//
//    // Compare the decoded file with the original input file
//    EXPECT_TRUE(compare_files(input_file_name, rsdecoded_file_name)) << "Decoded output does not match the original input.";
//
//    // Close the logger
//    wd_codec::Logger::close();
//}

//// Helper function to compare files


//// Google Test Case
//TEST(EncoderDecoderTests, FullEncodeDecodeCycle) {
//    // Initialize the logger
//    wd_codec::Logger::init("logfile.txt");
//
//    // Finite Field and Reed-Solomon Parameters
//    const std::size_t field_descriptor = 8;
//    const std::size_t generator_polynomial_index = 120;
//    const std::size_t generator_polynomial_root_count = 32;
//    const std::size_t code_length = 255;
//    const std::size_t fec_length = 32;
//    const std::size_t data_length = code_length - fec_length;
//    const std::size_t stack_size = 255;
//
//    const std::string input_file_name = "input.dat";
//    const std::string rsencoded_output_file_name = "output.rsenc";
//    const std::string rsdecoded_file_name = "output.rsdec";
//
//    // Create a valid input file
//    create_file(input_file_name, data_length * stack_size - 3);
//
//    // Instantiate Finite Field and Generator Polynomials
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
//        FAIL() << "Error generating G(X)";
//    }
//    wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", generator_polynomial);
//
//    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
//    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;
//
//    const encoder_t encoder(field, generator_polynomial);
//    const decoder_t decoder(field, generator_polynomial_index);
//
//    // Perform encoding
//    wd_codec::reed_solomon::File_Encoder<code_length, fec_length>(
//        encoder,
//        input_file_name,
//        rsencoded_output_file_name
//    );
//
//    // Introduce errors into the encoded file
//    wd_codec::error_injection::inject_random_errors<code_length, fec_length>(rsencoded_output_file_name, 50);
//
//    bool decode_success = false;
//
//    // Perform decoding
//    wd_codec::reed_solomon::File_Decoder<code_length, fec_length>(
//        decoder,
//        rsencoded_output_file_name,
//        rsdecoded_file_name
//    );
//            // Check if the decoded file is empty
//        std::ifstream decoded_file(rsdecoded_file_name, std::ios::binary);
//        decode_success = decoded_file.good() && decoded_file.peek() == std::ifstream::traits_type::eof();

//    // Compare the decoded file with the original input file
//    EXPECT_FALSE(decode_success) << "Decoded output does not match the original input.";
//
//    // Close the logger
//    wd_codec::Logger::close();
//}


//
//// Helper function to compare files
//bool compare_files(const std::string& file1, const std::string& file2) {
//    std::ifstream f1(file1, std::ios::binary | std::ios::ate);
//    std::ifstream f2(file2, std::ios::binary | std::ios::ate);
//
//    if (!f1.good() || !f2.good()) return false;
//
//    auto size1 = f1.tellg();
//    auto size2 = f2.tellg();
//
//    if (size1 != size2) return false;
//
//    f1.seekg(0, std::ios::beg);
//    f2.seekg(0, std::ios::beg);
//
//    std::vector<char> buffer1(size1);
//    std::vector<char> buffer2(size2);
//
//    f1.read(buffer1.data(), size1);
//    f2.read(buffer2.data(), size2);
//
//    return std::equal(buffer1.begin(), buffer1.end(), buffer2.begin());
//}
//
//// Google Test Case
//TEST(EncoderDecoderTests, HandleTooManyErrors) {
//    // Initialize the logger
//    wd_codec::Logger::init("logfile.txt");
//
//    // Finite Field and Reed-Solomon Parameters
//    const std::size_t field_descriptor = 8;
//    const std::size_t generator_polynomial_index = 120;
//    const std::size_t generator_polynomial_root_count = 32;
//    const std::size_t code_length = 255;
//    const std::size_t fec_length = 32;
//    const std::size_t data_length = code_length - fec_length;
//    const std::size_t stack_size = 255;
//
//    const std::string input_file_name = "input.dat";
//    const std::string rsencoded_output_file_name = "output.rsenc";
//    const std::string rsdecoded_file_name = "output.rsdec";
//
//    // Create a valid input file
//    create_file(input_file_name, data_length * stack_size - 3);
//
//    // Instantiate Finite Field and Generator Polynomials
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
//        FAIL() << "Error generating G(X)";
//    }
//    wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", generator_polynomial);
//
//    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
//    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;
//
//    const encoder_t encoder(field, generator_polynomial);
//    const decoder_t decoder(field, generator_polynomial_index);
//
//    // Perform encoding
//    wd_codec::reed_solomon::File_Encoder<code_length, fec_length>(
//        encoder,
//        input_file_name,
//        rsencoded_output_file_name
//    );
//
//    // Introduce more errors than the code can correct
//    wd_codec::error_injection::inject_random_errors<code_length, fec_length>(rsencoded_output_file_name, 50);
//
//    // Perform decoding
//    wd_codec::reed_solomon::File_Decoder<code_length, fec_length>(
//        decoder,
//        rsencoded_output_file_name,
//        rsdecoded_file_name
//    );
//
//    // Check if the decoded file is empty or corrupted
//    std::ifstream decoded_file(rsdecoded_file_name, std::ios::binary);
//    bool decode_success = decoded_file.good() && decoded_file.peek() != std::ifstream::traits_type::eof();
//
//    // Compare the decoded file with the original input file
//    EXPECT_FALSE(decode_success) << "Decoder was expected to fail with too many errors.";
//
//    // Close the logger
//    wd_codec::Logger::close();
//}


// Test case for handling invalid Reed-Solomon parameters
TEST(EncoderDecoderTests, HandleInvalidRSParameters) {
    setupTestEnvironment();

    const std::string input_file_name = "input.dat";
    const std::string rsencoded_output_file_name = "output.rsenc";
    const std::string rsdecoded_file_name = "output.rsdec";

    // Create a simple input file
    createEmptyFile(input_file_name);

    // Define Reed-Solomon parameters with an invalid setup
    const std::size_t field_descriptor = 8;
    const std::size_t generator_polynomial_index = 999;  // Invalid index (typically out of range)
    const std::size_t generator_polynomial_root_count = 32;
    const std::size_t code_length = 255;
    const std::size_t fec_length = 260; // Invalid FEC length, exceeds code length
    const std::size_t data_length = code_length - fec_length;

    // Instantiate Finite Field and Generator Polynomials
    const wd_codec::galois::Field field(field_descriptor,
        wd_codec::galois::primitive_polynomial_size06,
        wd_codec::galois::primitive_polynomial06);
    wd_codec::galois::Polynomial generator_polynomial(field);

    // Attempt to generate G(X) and expect failure
    bool generator_polynomial_success = wd_codec::create_root_generator_polynomial(field,
        generator_polynomial_index,
        generator_polynomial_root_count,
        generator_polynomial);

    EXPECT_FALSE(generator_polynomial_success) << "Generator polynomial creation should fail with invalid parameters.";

    if (generator_polynomial_success) {
        // If the generator polynomial was created (unexpectedly), proceed to test encoder/decoder creation
        try {
            typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
            typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;

            const encoder_t encoder(field, generator_polynomial);
            const decoder_t decoder(field, generator_polynomial_index);

            // Attempt to perform encoding and decoding
            wd_codec::reed_solomon::File_Encoder<code_length, fec_length>(
                encoder,
                input_file_name,
                rsencoded_output_file_name
            );

            wd_codec::reed_solomon::File_Decoder<code_length, fec_length>(
                decoder,
                rsencoded_output_file_name,
                rsdecoded_file_name
            );

            FAIL() << "Encoder/decoder creation should fail with invalid parameters.";
        }
        catch (const std::exception& e) {
            // Expect an exception to be thrown due to invalid parameters
            SUCCEED() << "Caught expected exception: " << e.what();
        }
    }

    teardownTestEnvironment();
}