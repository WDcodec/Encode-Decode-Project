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

	// Check if the encode/decode process correctly handled the empty file
	EXPECT_FALSE(encode_success) << "Encoding should fail or handle an empty input file correctly.";
	EXPECT_FALSE(decode_success) << "Decoding should handle an empty file appropriately.";
	// Close the logger
	wd_codec::Logger::close();
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

//test the encoder with data larger than k
TEST(EncoderDecoderTests, BiggerThanK) {
	wd_codec::setup();
	const std::string input_file_name = "input.dat";
	const std::string rsencoded_output_file_name = "output_encode.txt";
	const std::string rsdecoded_file_name = "output_decode.txt";
	// Create a valid input file bigger than k
	wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
	const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);

	wd_codec::file_encoder_t file_encoder(encoder);

	if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
		std::cout << "Encoding failed." << std::endl;
		return;
	}
	// Compare the decoded file with the original input file
	std::size_t expected_block_count = wd_codec::fileio::file_size(input_file_name) / wd_codec::data_length;
	EXPECT_EQ(expected_block_count, file_encoder.get_blocks_number()) << "expected block count is different than the actual blocks number.";
}

//test UnCorruptedEncodeDecodeCycle
TEST(EncoderDecoderTests, UnCorruptedEncodeDecodeCycle) {

	wd_codec::setup();
	const std::string input_file_name = "input.dat";
	const std::string rsencoded_output_file_name = "output_encode.txt";
	const std::string rsdecoded_file_name = "output_decode.txt";

	// Create a valid input file
	wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * wd_codec::stack_size - 3);
	wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);
	wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

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

//TODO: seperate all the UT to diffrent file
//TODO: extract all the defentions and function outsize the test to test utility
//TODO: add unit test for picture.
const std::string rsencoded_output_file_name_with_residue = "output_encode.txt";

TEST(EncoderDecoderTests, NonDivisibleInputByK) {
	wd_codec::setup();
	const std::string input_file_name = "input.dat";
	// Create a valid input file bigger than k
	wd_codec::fileio::create_file(input_file_name, wd_codec::data_length * (wd_codec::stack_size - 1) + 200);
	const wd_codec::Encoder encoder(wd_codec::field, wd_codec::generator_polynomial);

	// Perform encoding
	wd_codec::file_encoder_t file_encoder(encoder);

	file_encoder.encode(input_file_name, rsencoded_output_file_name_with_residue);

	bool is_residue_handled = file_encoder.get_is_residue_handled();
	EXPECT_TRUE(is_residue_handled) << "Encode does not handle the last smaller than k block.";


	// Close the logger
	wd_codec::Logger::close();
}

TEST(EncoderDecoderTests, NonDivisibleInputByN) {
	wd_codec::setup();
	const std::string rsdecoded_file_name = "output_decode.txt";
	const wd_codec::Decoder decoder(wd_codec::field, wd_codec::generator_polynomial_index);

	// Perform encoding
	wd_codec::file_decoder_t file_decoder(decoder);

	file_decoder.decode(rsencoded_output_file_name_with_residue, rsdecoded_file_name);

	bool is_residue_handled = file_decoder.get_is_residue_handled();
	EXPECT_TRUE(is_residue_handled) << "Decode does not handle the last smaller than n block.";


	// Close the logger
	wd_codec::Logger::close();
}


TEST(EncoderDecoderTests, HandleTooManyErrors) {
    // Initialize the logger


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

    // Introduce more errors than the code can correct
	if (!file_encoder.encode(input_file_name, rsencoded_output_file_name)) {
		std::cout << "Encoding failed." << std::endl;
		return;
	}
	wd_codec::error_injection::inject_random_errors<wd_codec::code_length, wd_codec::fec_length>(rsencoded_output_file_name,50);
	bool decode_success = file_decoder.decode(rsencoded_output_file_name, rsdecoded_file_name);



    // Compare the decoded file with the original input file
    EXPECT_FALSE(decode_success) << "Decoder was expected to fail with too many errors.";
	
	// Close the logger
	wd_codec::Logger::close();

}


