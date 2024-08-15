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
			
			bool encode(block_type& rsblock) const;
			const bool encoder_valid;
			const galois::Field& field_;
			const galois::Polynomial generator_;
		};
	}
}



