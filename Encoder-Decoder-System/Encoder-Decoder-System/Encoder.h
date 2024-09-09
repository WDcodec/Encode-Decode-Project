#pragma once
#include <cassert>
#include <iostream>
#include <vector>

#include "Field.h"
#include "Polynomial.h"
#include "Field_Element.h"
#include "Block.h"
#include "Logger.h"
namespace wd_codec
{

	namespace reed_solomon
	{
		template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
		class Encoder
		{
		public:
			typedef Block<code_length, fec_length> block_type;

			Encoder(const galois::Field& gfield, const galois::Polynomial& generator)
				: encoder_valid(code_length == gfield.size()),
				field_(gfield),
				generator_(generator)
			{}

			~Encoder()
			{}

			//Encoding the message.
			bool encode(block_type& rsblock) const {
				//Computing data = p(x)*x^n-k,
				//where p(x) in rsblock.
				galois::Polynomial data(field_, code_length);
				place_data_in_codeword(rsblock, data);
				//remainder = r(x) = p(x)*x^n-k % g(x).
				const galois::Polynomial remainder = data % generator_;
				const galois::field_symbol     mask = field_.mask();
				////fec_length = n-k, to make sure the remainder is valid.
				if (remainder.deg() == (fec_length - 1))
				{
					//make rsblock = (reversed) s(x) =  p(x)*x^n-k - r(x)
					for (std::size_t i = 0; i < fec_length; ++i)
					{
						//rsblock.fec(i) = rsblock[k+i]
						rsblock.fec(i) = remainder[fec_length - 1 - i].poly() & mask;
					}
				}
				else
				{
					/*
					   Note: Encoder should never branch here.
					   Possible issues to look for:
					   1. Generator polynomial degree is not equivelent to fec length
					   2. Field and code length are not consistent.

					*/

					wd_codec::Logger::log(wd_codec::ERROR, "Encode: Encode failed!");
					return false;
				}
                #ifdef DEBUG
				wd_codec::Logger::log(wd_codec::INFO, "Encode: Encode succeeded");
                #endif // DEBUG
				return true;

			}

			bool encode(block_type& rsblock, const std::string& data) const {
				const galois::field_symbol  mask = field_.mask();
				//iterate the data polynomial and add it to the rsblock data.
				for (std::size_t i = 0; i < data_length; i++) {
					rsblock[i] = data[i] & mask;
				}
				return encode(rsblock);
			}

		private:
			const bool encoder_valid;
			const galois::Field& field_;
			const galois::Polynomial generator_;

			//Compute the P(x)*x^(n-k) for making place to the redundancy. 
			inline void place_data_in_codeword(const block_type& rsblock, galois::Polynomial& data) const {
				for (std::size_t i = code_length - 1; i >= fec_length; i--) {
					data[i] = rsblock[code_length - 1 - i];
				}
			}

		};
	}
}



