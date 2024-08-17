#pragma once
#include <cassert>
#include <iostream>
#include <vector>

#include "Field.h"
#include "Polynomial.h"
#include "Field_Element.h"
#include "Block.h"
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
			
			bool encode(block_type& rsblock) const {
				//remainder = r(x) = p(x)*x^n-k % g(x)
				const galois::Polynomial remainder = msg_poly(rsblock) % generator_;
				const galois::field_symbol     mask = field_.mask();
				//fec_length = n-k
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
					rsblock.error = block_type::e_encoder_error1;
					return false;
				}

				return true;

			}
			const bool encoder_valid;
			const galois::Field& field_;
			const galois::Polynomial generator_;
		};
	}
}



