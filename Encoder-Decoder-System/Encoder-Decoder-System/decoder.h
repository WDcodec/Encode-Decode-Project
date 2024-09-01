#pragma once
#include <vector>
#include<iostream>

#include "Field.h"
#include "Polynomial.h"
#include "Block.h"
//#include "Traits"

namespace wd_codec {

	namespace reed_solomon {
		template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>// n-data+redundancy ,redundancy length, k-data length

		class Decoder
		{

		public:
			//typedef Traits::reed_solomon_triat<code_length, fec_length, data_length> trait;//n, k , n-k
			typedef Block<code_length, fec_length> block_type;                             //n,k
			Decoder(const galois::Field& field, const unsigned int& gen_initial_index = 0);//get  encoded polynomial.
			bool decode(Block& rsblock) const {
				//TODO: handle the erasure_list
				if (!decoder_valid_) {
					rsblock.errors_detected = 0;
					rsblock.errors_corrected = 0;
					rsblock.zero_numerators = 0;
					rsblock.unrecoverable = true;
					rsblock.error = block_type::e_decoder_error0;

					return false;
				}

				galois::Polynomial received(field_, code_length - 1);

				load_message(received, rsblock);
				galois::Polynomial syndrome(field_);

				//if the syndrom is zero - no error detected!!
				if (compute_syndrome(received, syndrome) == 0) {
					rsblock.errors_detected = 0;
					rsblock.errors_corrected = 0;
					rsblock.zero_numerators = 0;
					rsblock.unrecoverable = false;

					return true;
				}

				//lamda is the error locator polynomial
				galois::field_polynomial lambda(galois::field_element(field_, 1));
				//using the berlekamp_massey_algorithm to compute the error locator polynomial (lamda)
				berlekamp_massey_algorithm(lambda, syndrome);

				std::vector<int> error_locations;
				//using the find_roots to compute the error locations using the lamda polynomial
				find_roots(lambda, error_locations);

				if (0 == error_locations.size())
				{
					/*
					  Syndrome is non-zero yet no error locations have
					  been obtained, conclusion:
					  It is possible that there are MORE errrors in the
					  message than can be detected and corrected for this
					  particular code.
					*/

					rsblock.errors_detected = 0;
					rsblock.errors_corrected = 0;
					rsblock.zero_numerators = 0;
					rsblock.unrecoverable = true;
					rsblock.error = block_type::e_decoder_error1;

					return false;
				}

				//correct the errors 
				return forney_algorithm(error_locations, lambda, syndrome, rsblock);

			}
		protected:
			bool                                  decoder_valid_;          //if decoder is properly initialized
			const galois::Field& field_;                  // used in decoding
			std::vector<galois::field_symbol>     root_exponent_table_;    // Stores root exponents for error correction
			std::vector<galois::field_symbol>     syndrome_exponent_table_;// Holds syndrome exponents for error detection
			std::vector<galois::Polynomial> gamma_table_;            // Contains gamma polynomials for error correction
			const galois::Polynomial        X_;                      // for error correction calculations in the Error locations
			const unsigned int                    gen_initial_index_;      //index for generator polynomial
		private:
			Decoder();
			Decoder(const Decoder& dec);
			Decoder& operator=(const Decoder& dec);
			~Decoder();
		};
	}
}

