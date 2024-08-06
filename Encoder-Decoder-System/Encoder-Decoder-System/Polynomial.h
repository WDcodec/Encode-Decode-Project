#pragma once
#include <vector>
#include <cassert>
#include <iostream>

#include "Field.h"
#include "Field_element.h"


namespace wd_codec
{

	namespace galois
	{

		class Polynomial
		{
			Field& field_;
			std::vector<Field_elements> poly_;

		};
	}
}
