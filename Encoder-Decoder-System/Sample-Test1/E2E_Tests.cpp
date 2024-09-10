//#include "pch.h"
////#include "gtest/gtest.h"
////TEST(TestCaseName, TestName) {
////  EXPECT_EQ(1, 1);
////  EXPECT_TRUE(true);
////}
#include <gtest/gtest.h>
#include "Test_Utility.h"
///Test case for handling an empty file
TEST(EncoderDecoderTests, HandleEmptyFile) {
	const std::string empty_file_name = "empty_input.dat";
	const std::string rsencoded_output_file_name = "empty_encode_output.txt";
	const std::string rsdecoded_file_name = "empty_decode_output.txt";

	// Create an empty file
	createEmptyFile(empty_file_name);
	wd_codec::setup();
	// Attempt to perform encoding and decoding on the empty file
	bool encode_success = false;
	bool decode_success = false;

	try {
		// Perform encoding
		const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
		const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

		wd_codec::file_encoder_t file_encoder(encoder);
		wd_codec::file_decoder_t file_decoder(decoder);

		// Check if the encoded file is empty
		std::ifstream encoded_file(rsencoded_output_file_name, std::ios::binary);
		encode_success = file_encoder.encode(empty_file_name, rsencoded_output_file_name) &&
			!(encoded_file.peek() == std::ifstream::traits_type::eof());

		// Check if the decoded file is empty
		std::ifstream decoded_file(rsdecoded_file_name, std::ios::binary);
		decode_success = file_decoder.decode(empty_file_name, rsencoded_output_file_name) &&
			!(decoded_file.peek() == std::ifstream::traits_type::eof());
	}
	catch (const std::exception& e) {
		// Log or handle the exception
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	// Check if the encode/decode process correctly handled the empty file
	EXPECT_FALSE(encode_success) << "Encoding should fail or handle an empty input file correctly.";
	EXPECT_FALSE(decode_success) << "Decoding should handle an empty file appropriately.";

	//teardownTestEnvironment();
}



//test end to end
TEST(EncoderDecoderTests, FullEncodeDecodeCycle) {
	wd_codec::setup();

	const std::string input_file_name = "input.dat";
	const std::string rsencoded_output_file_name = "output_encode.txt";
	const std::string rsdecoded_file_name = "output_decode.txt";
	// Create a valid input file
	wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
	const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
	const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

	// Perform encoding
	wd_codec::file_encoder_t file_encoder(encoder);
	wd_codec::file_decoder_t file_decoder(decoder);
	if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
		std::cout << "Encoding failed." << std::endl;
		return;
	}
	wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name);
	if (!file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name)) {
		std::cout << "Decoding failed." << std::endl;
		return;
	}
	// Compare the decoded file with the original input file
	EXPECT_TRUE(compare_files(input_file_name, rsdecoded_file_name)) << "Decoded output does not match the original input.";

	// Close the logger
	wd_codec::Logger::close();
}

//test end to end
TEST(EncoderDecoderTests, BiggerThanK) {
	wd_codec::setup();
	const std::string input_file_name = "input.dat";
	const std::string rsencoded_output_file_name = "output_encode.txt";
	const std::string rsdecoded_file_name = "output_decode.txt";
	// Create a valid input file bigger than k
	wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
	const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);

	// Perform encoding
	wd_codec::file_encoder_t file_encoder(encoder);

	if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
		std::cout << "Encoding failed." << std::endl;
		return;
	}
	// Compare the decoded file with the original input file
	std::size_t expected_block_count = wd_codec::fileio::file_size(input_file_name) / wd_codec::data_length;
	EXPECT_EQ(expected_block_count, file_encoder.get_blocks_number()) << "expected block count is different than the actual blocks number.";
	// Close the logger
	wd_codec::Logger::close();
}
TEST(EncoderDecoderTests, UnCorruptedEncodeDecodeCycle) {
#define VALID_TEST 

	wd_codec::setup();

	const std::string input_file_name = "input.dat";
	const std::string rsencoded_output_file_name = "output_encode.txt";
	const std::string rsdecoded_file_name = "output_decode.txt";

	// Create a valid input file
	wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
	wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
	wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

	// Perform encoding
	wd_codec::file_encoder_t file_encoder(encoder);
	wd_codec::file_decoder_t file_decoder(decoder);

	if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
		std::cout << "Encoding failed." << std::endl;
		return;
	}

	bool decode_success = file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name);

	// Check if the decoding was successful
	EXPECT_TRUE(decode_success) << "Decoding failed or syndrome is non-zero.";
	EXPECT_TRUE(decoder.out_in_syndrom) << "Syndrome check in decode process was not triggered as expected.";
	// Close the logger
	wd_codec::Logger::close();
}



//TEST(EncoderDecoderTests, FullEncodeDecodeCycle) {
//
//
//
//	const std::string input_file_name = "input.dat";
//	const std::string rsencoded_output_file_name = "output.rsenc";
//	const std::string rsdecoded_file_name = "output.rsdec";
//
//	// Create a valid input file
//	wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
//
//
//	const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
//	const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);
//	wd_codec::file_encoder_t file_encoder(encoder);
//	wd_codec::file_decoder_t file_decoder(decoder);
//	if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
//		std::cout << "Encoding failed." << std::endl;
//		return;
//	}
//	wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name, 50);
//	if (!file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name)) {
//		std::cout << "Decoding failed." << std::endl;
//		return;
//	}
//	// Perform encoding
//
//	// Introduce errors into the encoded file
//
//	bool decode_success = false;
//	// Check if the decoded file is empty
//	std::ifstream decoded_file(rsdecoded_file_name, std::ios::binary);
//	decode_success = decoded_file.good() && decoded_file.peek() == std::ifstream::traits_type::eof();
//
//	// Compare the decoded file with the original input file
//	EXPECT_FALSE(decode_success) << "Decoded output does not match the original input.";
//
//	// Close the logger
//	wd_codec::Logger::close();
//}


//// Google Test Case
//TEST(EncoderDecoderTests, FullEncodeDecodeCycle) {
//    wd_codec::setup();
// 
//    const std::string input_file_name = "input.dat";
//    const std::string rsencoded_output_file_name = "output.rsenc";
//    const std::string rsdecoded_file_name = "output.rsdec";
//
//    // Create a valid input file
//    wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
//
//    const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
//    const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);
//    wd_codec::file_encoder_t file_encoder(encoder);
//    wd_codec::file_decoder_t file_decoder(decoder);
//
//    // Perform encoding
//    ASSERT_TRUE(file_encoder.encode(input_file_name, rsencoded_output_file_name)) << "Encoding failed.";
//
//    // Introduce errors into the encoded file
//    wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name, 100);
//
//    // Perform decoding
//    bool decode_success = file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name);
//
//    // Since too many errors are expected, decoding should fail
//    EXPECT_FALSE(decode_success) << "Decoding should fail due to too many errors.";
//
//    // Optional: If decoding does not fail, check that the decoded file does not match the original
//    if (decode_success) {
//        std::ifstream original_file(input_file_name, std::ios::binary);
//        std::ifstream decoded_file(rsdecoded_file_name, std::ios::binary);
//
//        std::string original_content((std::istreambuf_iterator<char>(original_file)), std::istreambuf_iterator<char>());
//        std::string decoded_content((std::istreambuf_iterator<char>(decoded_file)), std::istreambuf_iterator<char>());
//
//        EXPECT_NE(original_content, decoded_content) << "Decoded output should not match the original input due to excessive errors.";
//    }
//
//    // Close the logger
//    wd_codec::Logger::close();
//}



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

//TEST(EncoderDecoderTests, HandleInvalidRSParameters) {
//    setupTestEnvironment();
//
//    const std::string input_file_name = "input.dat";
//    const std::string rsencoded_output_file_name = "output.rsenc";
//    const std::string rsdecoded_file_name = "output.rsdec";
//
//    // Create a simple input file
//    wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
//
//
//    // Define Reed-Solomon parameters with an invalid setup
//     std::size_t field_descriptor = 8;
//    const std::size_t generator_polynomial_index = 999;  // Invalid index (typically out of range)
//    const std::size_t generator_polynomial_root_count = 32;
//    const std::size_t code_length = 255;
//    const std::size_t fec_length = 260; // Invalid FEC length, exceeds code length
//    const std::size_t data_length = code_length - fec_length;
//
//    // Instantiate Finite Field and Generator Polynomials
//    const wd_codec::galois::Field field(field_descriptor,
//        wd_codec::galois::primitive_polynomial_size06,
//        wd_codec::galois::primitive_polynomial06);
//    wd_codec::galois::Polynomial generator_polynomial(field);
//
//    // Attempt to generate G(X) and expect failure
//    bool generator_polynomial_success = wd_codec::create_root_generator_polynomial(field,
//        generator_polynomial_index,
//        generator_polynomial_root_count,
//        generator_polynomial);
//
//    EXPECT_FALSE(generator_polynomial_success) << "Generator polynomial creation should fail with invalid parameters.";
//
//    if (generator_polynomial_success) {
//        // If the generator polynomial was created (unexpectedly), proceed to test encoder/decoder creation
//        try {
//            typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> encoder_t;
//            typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> decoder_t;
//            typedef wd_codec::reed_solomon::File_Encoder<code_length, fec_length> file_encoder_t;
//            typedef wd_codec::reed_solomon::File_Decoder<code_length, fec_length> file_decoder_t;
//            const encoder_t encoder(field, generator_polynomial);
//            const decoder_t decoder(field, generator_polynomial_index);
//
//            // Attempt to perform encoding and decoding
//            file_encoder_t file_encoder(encoder);
//            file_decoder_t file_decoder(decoder);
//            if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
//                std::cout << "Encoding failed." << std::endl;
//                return;
//            }
//            wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name);
//            if (!file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name)) {
//                std::cout << "Decoding failed." << std::endl;
//                return;
//            }
//
//            FAIL() << "Encoder/decoder creation should fail with invalid parameters.";
//        }
//        catch (const std::exception& e) {
//            // Expect an exception to be thrown due to invalid parameters
//            SUCCEED() << "Caught expected exception: " << e.what();
//        }
//    }
//
//    teardownTestEnvironment();
//}