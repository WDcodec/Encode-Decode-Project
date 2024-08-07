#pragma once
#include <vector>
#include <cassert>
#include <iostream>

#include "Field.h"
#include "Field_Element.h"


namespace wd_codec
{

	namespace galois
	{

		class Polynomial
		{
			Field& field_;
			std::vector<Field_Element> poly_;

		};
	}
}
