#include "pch.h"
#include "Test_Utility.h"


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

TEST(EncoderDecoderTests, PolynomialMultiplicationinGaloisField) {
	// P(x) = x^2 + x + 1
	wd_codec::galois::Field_Element P_elements[] = {   
		wd_codec::galois::Field_Element(wd_codec::field, 1),
	    wd_codec::galois::Field_Element(wd_codec::field, 1),
	    wd_codec::galois::Field_Element(wd_codec::field, 1)
	};

	// Q(x) = 2x + 2
	wd_codec::galois::Field_Element Q_elements[] = {
		wd_codec::galois::Field_Element(wd_codec::field, 2),
		wd_codec::galois::Field_Element(wd_codec::field, 2)
	};

	// R(x) = 2x^3 + 4x^2 + 3x + 2
	wd_codec::galois::Field_Element expected_coeffs[] = {
		wd_codec::galois::Field_Element(wd_codec::field, 2), 
		wd_codec::galois::Field_Element(wd_codec::field, 0),
		wd_codec::galois::Field_Element(wd_codec::field, 0),
		wd_codec::galois::Field_Element(wd_codec::field, 2)
	};

	wd_codec::galois::Polynomial P(wd_codec::field, 2 ,P_elements);
	wd_codec::galois::Polynomial Q(wd_codec::field, 1 , Q_elements);
	wd_codec::galois::Polynomial expected_result(wd_codec::field, 3, expected_coeffs);
	wd_codec::galois::Polynomial result(wd_codec::field);

	result = P * Q;
	EXPECT_EQ(result, expected_result);

	P *= Q;
	EXPECT_EQ(P, result);

}

