#include "Field_Element.h"
#include <iostream>


namespace wd_codec {

	namespace galois {



		inline wd_codec::galois::Field_Element operator + (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b) {
			Field_Element result = a;
			result += b;
			return result;
		}
		inline wd_codec::galois::Field_Element operator - (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b) {
			Field_Element result = a;
			result -= b;
			return result;
		}
		inline wd_codec::galois::Field_Element operator * (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b) {
			Field_Element result = a;
			result *= b;
			return result;
		}
		inline wd_codec::galois::Field_Element operator * (const wd_codec::galois::Field_Element& a, const wd_codec::galois::field_symbol& b) {
			Field_Element result = a;
			result *= b;
			return result;
		}
		inline wd_codec::galois::Field_Element operator * (const wd_codec::galois::field_symbol& a, const wd_codec::galois::Field_Element& b) {
			Field_Element result = b;
			result *= a;
			return result;
		}
		inline wd_codec::galois::Field_Element operator / (const wd_codec::galois::field_symbol& a, const wd_codec::galois::Field_Element& b) {
			Field_Element result = b;
			result /= a;
			return result;
		}
		inline wd_codec::galois::Field_Element operator ^ (const wd_codec::galois::Field_Element& a, const int& b) {
			Field_Element result = a;
			result ^= b;
			return result;
		}
		std::ostream& operator << (std::ostream& os, const Field_Element& gfe) {
			return os << gfe.poly_value_;
		}

	
	}
}

