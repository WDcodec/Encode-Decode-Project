#pragma once
#include <vector>
#include<iostream>

#include "Field.h"
#include "Polynomial.h"
#include "Logger.h"
//#include "Block.h"
//#include "Traits"

namespace wd_codec {

	namespace reed_solomon {
		template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>// n-data+redundancy ,redundancy length, k-data length

		class Decoder
		{

		public:
			//typedef Traits::reed_solomon_triat<code_length, fec_length, data_length> trait;//n, k , n-k
			//typedef Block<code_length, fec_length> block_type;                             //n,k
			Decoder(const galois::Field& field, const unsigned int& gen_initial_index = 0):
				decoder_valid_(field.size() == code_length),
                field_(field),
				gen_initial_index_(gen_initial_index),
				X_(wd_codec::galois::generate_X(field_))
			{
				if (decoder_valid_) {
					create_lookup_tables();
				}
			}
		protected:
			bool                                  decoder_valid_;          //if decoder is properly initialized
			const galois::Field&                  field_;                  // used in decoding
			std::vector<galois::field_symbol>     root_exponent_table_;    // Stores root exponents for error correction
			std::vector<galois::field_symbol>     syndrome_exponent_table_;// Holds syndrome exponents for error detection
			//std::vector<galois::Polynomial>       gamma_table_;            // Contains gamma polynomials for error correction
			const galois::Polynomial              X_;                      // for error correction calculations in the Error locations
			const unsigned int                    gen_initial_index_;      //index for generator polynomial
			void create_lookup_tables()
			{
				//root Exponent  : used in chien search to find error positionsby evaluating the error locator polynomial
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
		private:
			Decoder();//not implemnted
			Decoder(const Decoder& dec);//not implemnted
			Decoder& operator=(const Decoder& dec);
			//~Decoder() {}innecessary 

			std::vector<int> CheainSreach(const wd_codec::galois::Polynomial lambda, std::vector<int>error_locations) {
				// Locate the exact positions of the errors
				error_locations.resize(0);
				const std::size_t lambda_degree = lambda.deg();
				for (int i = 1; i < = code_length; ++i) {
					if (0 == poly(field_.alpha(i)).poly()) {
						error_locations.push_back(i);
						if (lambda_degree == error_locations.size()) {
							break;
						}
					}
				}
				wd_codec::Logger::log(wd_codec::INFO, "Decoder - CheainSreach()  completed");
				return error_locations;
			}
		};

		
	}
}

