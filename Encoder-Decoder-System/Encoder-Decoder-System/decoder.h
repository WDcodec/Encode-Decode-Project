#pragma once
#include <vector>
#include<iostream>

#include "Field.h"
#include "Polynomial.h"
#include "Logger.h"
#include "Block.h"

namespace wd_codec {

	namespace reed_solomon {
		template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>// n-data+redundancy ,redundancy length, k-data length

		class Decoder
		{

		public:
			//typedef Traits::reed_solomon_triat<code_length, fec_length, data_length> trait;//n, k , n-k
			typedef Block<code_length, fec_length> block_type;                             //n,k
			Decoder(const galois::Field& field, const unsigned int& gen_initial_index = 0):
				decoder_valid_(field.size() == code_length),
                field_(field),
				gen_initial_index_(gen_initial_index),
				X_(wd_codec::galois::generate_X(field_))
			{
				Logger::log(wd_codec::INFO, " Start Generate Decoder.");

				if (decoder_valid_) {
					create_lookup_tables();
				}
			}

			//Function that try to detect and correct the errors
			bool decode(block_type& rsblock) const {
				Logger::log(wd_codec::INFO, " Start  decode function.");

				//TODO: handle the erasure_list
				if (!decoder_valid_) {
					rsblock.errors_detected = 0;
					rsblock.errors_corrected = 0;
					rsblock.zero_numerators = 0;
					rsblock.unrecoverable = true;
					//rsblock.error = block_type::e_decoder_error0;

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
				galois::Polynomial lambda(galois::Field_Element(field_, 1));
				//using the berlekamp_massey_algorithm to compute the error locator polynomial (lamda)
				berlekamp_massey_algorithm(lambda, syndrome);

				std::vector<int> error_locations;
				//using the chien sreach to compute the error locations using the lamda polynomial
				chien_sreach(lambda, error_locations);

				if (0 == error_locations.size())
				{
					/*
					  Syndrome is non-zero yet and no error locations have
					  been obtained, conclusion:
					  It is possible that there are MORE errrors in the
					  message than can be detected and corrected for this
					  particular code.
					*/

					rsblock.errors_detected = 0;
					rsblock.errors_corrected = 0;
					rsblock.zero_numerators = 0;
					rsblock.unrecoverable = true;
					//rsblock.error = block_type::e_decoder_error1;

					return false;
				}
				else
					rsblock.errors_detected = error_locations.size();
				//correct the errors 
				return forney_algorithm(error_locations, lambda, syndrome, rsblock);

			}

			//Function that compute the result of placing all the generate roots in the recieved poly.
			int compute_syndrome(const galois::Polynomial& received,galois::Polynomial& syndrome) const
			{
			//	Logger::log(wd_codec::INFO, " Start computing syndrome.");

				int error_flag = 0;
				syndrome = galois::Polynomial(field_, fec_length - 1);
				for (std::size_t i = 0; i < fec_length; i++)
				{
					//syndrome[i] equal to the result of place root_i (syndrome_exponent_table_[i]) in the received data.
					syndrome[i] = received(syndrome_exponent_table_[i]);
					error_flag |= syndrome[i].poly();
				}

				return error_flag;
			}

			// Function that Compute the magnitudes of the errors and the correct the data
			bool forney_algorithm(const std::vector<int>& error_locations, const galois::Polynomial& lambda,
				                  const galois::Polynomial& syndrome, block_type& rsblock) const
			{
				Logger::log(wd_codec::INFO, " forney_algorithm.");

				/* the forney algorthim is a formula that find the correct data base on the lambda, syndrom and error_locations*/
				galois::Polynomial omega = (lambda * syndrome) % fec_length;
				galois::Polynomial lambda_derivative = lambda.derivative();
				std::cout << "\nomega: " << omega;
				std::cout << "\nlambda_derivative: " << lambda_derivative;
				rsblock.errors_corrected = 0;
				rsblock.zero_numerators = 0;
				// pass over all the error_locations in E(x).
				for (const unsigned int error_location : error_locations) {
					// compute the polynomial expression alpha at the error location (alpha^E[i])
					// TODO: check how is it inversed
					const galois::field_symbol alpha_inverse = field_.alpha(error_location);
					const galois::field_symbol numerator = (omega(alpha_inverse) * root_exponent_table_[error_location]).poly();
					const galois::field_symbol denominator = lambda_derivative(alpha_inverse).poly();
					if (numerator != 0) {
						if (denominator != 0) {
							std::cout << "\nbefor: " << char(rsblock[error_location - 1]) << " ";
							rsblock[error_location - 1] ^= field_.div(numerator, denominator);
							std::cout << "after: " << char(rsblock[error_location - 1]) << " ";
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
			void create_lookup_tables()
			{
				Logger::log(wd_codec::INFO, " create_lookup_tables.");

				//root Exponent  : used in chien search , forney algo' to find error positionsby evaluating the error locator polynomial
				root_exponent_table_.reserve(field_.size() + 1);
				for (int i = 0; i < static_cast<int>(field_.size() + 1); ++i)
				{
					root_exponent_table_.push_back(field_.exp(field_.alpha(code_length - i), (1 - gen_initial_index_)));
				}
				//Syndrome Exponent : used to compute syndromes for error detection.
				syndrome_exponent_table_.reserve(fec_length);
				for (int i = 0; i < static_cast<int>(fec_length); ++i)
				{
					syndrome_exponent_table_.push_back(field_.alpha(gen_initial_index_ + i));
				}
				wd_codec::Logger::log(wd_codec::INFO, " decoder  - create lookup tables succeeded");
				//TODO:ERASSURE BUILT TABLE
			}

			void chien_sreach(const galois::Polynomial& lambda, std::vector<int>& error_locations) const {
				// Locate the exact positions of the errors
				error_locations.reserve(fec_length << 1);
				error_locations.resize(0);
				const std::size_t lambda_degree = lambda.deg();
				for (int i = 1; i <= static_cast<int>(code_length); ++i) {
					if (0 == lambda(field_.alpha(i)).poly()) {
						error_locations.push_back(i);
						if (lambda_degree == error_locations.size()) {
							break;
						}
					}
				}
				wd_codec::Logger::log(wd_codec::INFO, "Decoder - CheainSreach()  completed");
				std::cout << "\nerror_locations: ";
				for (int x : error_locations) {
					std::cout << x << " ";
				}
			}

			void compute_discrepancy(galois::Field_Element& discrepancy,
				const galois::Polynomial& lambda,
				const galois::Polynomial& syndrome,
				const std::size_t& l,
				const std::size_t& round) const
			{
				/*
				   Compute the lambda discrepancy at the current round of BMA
				*/

				const std::size_t upper_bound = std::min(static_cast<int>(l), lambda.deg());

				discrepancy = 0;

				for (std::size_t i = 0; i <= upper_bound; ++i)
				{  //like c(j)*s(i-j)
					discrepancy += lambda[i] * syndrome[round - i];
				}
			}

			void berlekamp_massey_algorithm(galois::Polynomial& lambda,
				const galois::Polynomial& syndrome) const
			{
				Logger::log(wd_codec::INFO, " berlekamp_massey_algorithm.");

				/*
				   Modified Berlekamp-Massey Algorithm
				   Identify the shortest length linear feed-back shift register (LFSR)
				   that will generate the sequence equivalent to the syndrome.
				*/
				//f - last faild
				int i = -1;
				//|c| - coeffictiont number
				std::size_t l = 0;
				//d - the c correcting
				galois::Field_Element discrepancy(field_, 0);
				//b
				galois::Polynomial previous_lambda = lambda << 1;

				for (std::size_t round = 0; round < fec_length; ++round)
				{
					//checking if the current d is good -  discrepancy = 0
					compute_discrepancy(discrepancy, lambda, syndrome, l, round);

					if (discrepancy != 0)
					{
						//computing c+d
						galois::Polynomial tau = lambda - (discrepancy * previous_lambda);

						if (static_cast<int>(l) < (static_cast<int>(round) - i))
						{
							const std::size_t tmp = round - i;
							i = static_cast<int>(round - l);
							l = tmp;
							//d = (old c sequence)/discrepancy count
							previous_lambda = lambda / discrepancy;
						}

						lambda = tau;
					}

					previous_lambda <<= 1;
				}
				std::cout << "lambda: " << lambda;
			}
		protected:
			void load_message(galois::Polynomial& received, const block_type& rsblock) const
			{

				//Load message data into received polynomial in reverse order.
				for (std::size_t i = 0; i < code_length; ++i)
				{
					received[code_length - 1 - i] = rsblock[i];
				}
			}
			bool                                  decoder_valid_;          //if decoder is properly initialized
			const galois::Field&                  field_;                  // used in decoding
			std::vector<galois::field_symbol>     root_exponent_table_;    // Stores root exponents for error correction
			std::vector<galois::field_symbol>     syndrome_exponent_table_;// Holds syndrome exponents for error detection
			//std::vector<galois::Polynomial>       gamma_table_;            // Contains gamma polynomials for error correction
			const galois::Polynomial               X_;                      // for error correction calculations in the Error locations
			const unsigned int                    gen_initial_index_;      //index for generator polynomial
			
		};

		
	}
}

