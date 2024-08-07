#pragma once
#include <vector>
#include<iostream>

#include "Field.h"
#include "Polynomial.h"
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
			Decoder(const galois::Field& field, const unsigned int& gen_initial_index = 0);//get  encoded polynomial.
		protected:
			bool                                  decoder_valid_;          //if decoder is properly initialized
			const galois::Field&                  field_;                  // used in decoding
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

