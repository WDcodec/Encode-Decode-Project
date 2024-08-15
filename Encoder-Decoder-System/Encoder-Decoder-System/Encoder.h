#pragma once
#include <cassert>
#include <iostream>
#include <vector>

#include "Field.h"
#include "Polynomial.h"
#include "Field_Element.h"

namespace wd_codec
{

	namespace galois
	{
		template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
		class Encoder
		{
			encoder(const galois::field& gfield, const galois::field_polynomial& generator)
				: encoder_valid_(code_length == gfield.size()),
				field_(gfield),
				generator_(generator)
			{}

			~encoder()
			{}
		
			bool encode(block_type& rsblock) const;
			const bool encoder_valid;
			const galois::Field& field_;
			const galois::Polynomial generator_;
		};
	}
}



