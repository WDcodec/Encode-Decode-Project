#include "Field_Element.h"
namespace wd_codec {
	namespace galois {


inline wd_codec::galois::Field_Element operator + (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b);
inline wd_codec::galois::Field_Element operator - (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b);
inline wd_codec::galois::Field_Element operator * (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b);
inline wd_codec::galois::Field_Element operator * (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b);
inline wd_codec::galois::Field_Element operator * (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b);
inline wd_codec::galois::Field_Element operator / (const wd_codec::galois::Field_Element& a, const wd_codec::galois::Field_Element& b);
inline wd_codec::galois::Field_Element operator ^ (const wd_codec::galois::Field_Element& a, const int& b);

//Compound Assignment Operators:

inline Field_Element& Field_Element::operator= (const Field_Element& gfe) {
	if ((this != &gfe) && (&this->field_ == &gfe.field_)) {
		this->poly_value_ = gfe.poly_value_;
	}
	return *this;
}

inline Field_Element& Field_Element::operator= (const field_symbol& value) {
	this->poly_value_ = value;
	return *this;
}

inline Field_Element& Field_Element::operator += (const Field_Element& gfe) {
	//xor does addition in galois fiels
	this->poly_value_ ^= gfe.poly_value_;
	return *this;
}

inline Field_Element& Field_Element::operator += (const field_symbol& value) {
	this->poly_value_ ^= value;
	return *this;
}

inline Field_Element& Field_Element::operator -= (const Field_Element& gfe) {
	//xor does subtraction in galois fiels
	this->poly_value_ ^= gfe.poly_value_;
	return *this;
}

inline Field_Element& Field_Element::operator -= (const field_symbol& value) {
	this->poly_value_ ^= value;
	return *this;
}

inline Field_Element& Field_Element::operator *= (const Field_Element& gfe) {
	this->poly_value_ = this->field_.mul(poly_value_, gfe.poly_value_);
	return *this;
}

inline Field_Element& Field_Element::operator *= (const field_symbol& value) {
	this->poly_value_ = this->field_.mul(poly_value_, value);
	return *this;
}

inline Field_Element& Field_Element::operator /= (const Field_Element& gfe) {
	this->poly_value_ = this->field_.div(poly_value_, gfe.poly_value_);
	return *this;
}

inline Field_Element& Field_Element::operator /= (const field_symbol& value) {
	this->poly_value_ = this->field_.div(poly_value_, value);
	return *this;
}


	}
}