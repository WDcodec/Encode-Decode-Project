#pragma once
#include <cassert>
#include <iostream>
#include <vector>

#include "Field.h"
#include "Polynomial.h"
#include "Field_element.h"

namespace wd_codec
{

	namespace galois
	{

		class Encoder
		{
			const bool encoder_valid;
			const galois::Field& field_;
			const galois::Polynomial generator_;
		};
	}
}

