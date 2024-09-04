#include "pch.h"
//#include "gtest/gtest.h"
//TEST(TestCaseName, TestName) {
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//}
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



