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

			//Function that compute the result of placing all the generate roots in the recieved poly.
			int compute_syndrome(const galois::Polynomial& received,
				galois::Polynomial& syndrome) const
			{
				int error_flag = 0;
				for (std::size_t i = 0; i < fec_length; i++)
				{
					//syndrome[i] equal to the result of place root_i (syndrome_exponent_table_[i]) in the received data.
					syndrome[i] = received(syndrome_exponent_table_[i]);
					error_flag |= syndrome[i].poly;
				}

				return error_flag;
			}

			// Function that Compute the magnitudes of the errors and the correct the data
			bool forney_algorithm(const std::vector<int>& error_locations, const galois::Polynomial& lambda,
				const galois::Polynomial& syndrome, block_type& rsblock) const
			{
				/* the forney algorthim is a formula that find the correct data base on the lambda, syndrom and error_locations*/
				galois::Polynomial& omega = (lambda * syndrome) % fec_length;
				galois::Polynomial& lambda_derivative = lambda.derivative();
				rsblock.errors_corrected = 0;
				rsblock.zero_numerators = 0;
				// pass over all the error_locations in E(x).
				for (const unsigned int error_location : error_locations) {
					// compute the polynomial expression alpha at the error location (alpha^E[i])
					// TODO: check how is it inversed
					const galois::field_symbol alpha_inverse = field_.alpha(error_location);
					const galois::field_symbol numerator = (omega(alpha_inverse) * root_exponent_table_[error_location]).poly();
					const galois::field_symbol denominator = lambda_derivative(alpha_inverse).poly;
					if (numerator != 0) {
						if (denominator != 0) {
							rsblock[error_location - 1] ^= field_.div(numerator, denominator);
							rsblock.errors_corrected++;
						}
						else {
							//if the denominator equal to 0, cannot evaluate
							//TODO: handle error.
							return false;
						}
					}
					else {
						rsblock.zero_numerators++;
					}

				}

				if (lambda.deg() == static_cast<int>(rsblock.errors_detected))
					return true;
				else
				{
					//TODO: handle error.
					return false;
				}
			}

		protected:
			bool                                  decoder_valid_;          //if decoder is properly initialized
			const galois::Field& field_;                                   // used in decoding
			std::vector<galois::field_symbol>     root_exponent_table_;    // Stores root exponents for error correction
			std::vector<galois::field_symbol>     syndrome_exponent_table_;// Holds syndrome exponents for error detection
			std::vector<galois::Polynomial>       gamma_table_;            // Contains gamma polynomials for error correction
			const galois::Polynomial              X_;                      // for error correction calculations in the Error locations
			const unsigned int                    gen_initial_index_;      //index for generator polynomial
		private:
			Decoder();
			Decoder(const Decoder& dec);
			Decoder& operator=(const Decoder& dec);
			~Decoder();
		};
	}
}

