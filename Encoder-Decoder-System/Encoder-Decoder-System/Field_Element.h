#pragma once

#include "Field.h"

namespace wd_codec {
	namespace galois {

		class Field_Element
		{
		public: 
			Field_Element(const Field& field, const field_symbol& value = 0);
			Field_Element(const Field_Element& element);
			~Field_Element() {};
		private:
			const Field& field_;
			//the value of the coafficient
			field_symbol poly_value_;

		};
	}
}

