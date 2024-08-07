#include "Field.h"

namespace wd_codec {

	namespace galois {
		//A function that normalizes the values that deviate from the field value range.
		inline field_symbol Field::normalize(field_symbol x) const {
			while (x < 0) {
				x += static_cast<field_symbol>(field_size_);
			}
			while (x > static_cast<field_symbol>(field_size_)) {
				x -= static_cast<field_symbol>(field_size_);
				//optimization of the typical normalization(using mod) to addition of div and mod
				x = (x >> power_) + (x & field_size_);
			}
			return x;
		}
	}
}

