#include "Field_Element.h"
#include <iostream>


namespace wd_codec {

	namespace galois {



		inline Field_Element operator + (const Field_Element& a, const Field_Element& b) {
			Field_Element result = a;
			result += b;
			return result;
		}
		inline Field_Element operator - (const Field_Element& a, const Field_Element& b) {
			Field_Element result = a;
			result -= b;
			return result;
		}
		inline Field_Element operator * (const Field_Element& a, const Field_Element& b) {
			Field_Element result = a;
			result *= b;
			return result;
		}
		inline Field_Element operator * (const Field_Element& a, const field_symbol& b) {
			Field_Element result = a;
			result *= b;
			return result;
		}
		inline Field_Element operator * (const field_symbol& a, const Field_Element& b) {
			Field_Element result = b;
			result *= a;
			return result;
		}
		inline Field_Element operator / (const field_symbol& a, const Field_Element& b) {
			Field_Element result = b;
			result /= a;
			return result;
		}
		inline Field_Element operator ^ (const Field_Element& a, const int& b) {
			Field_Element result = a;
			result ^= b;
			return result;
		}
		std::ostream& operator << (std::ostream& os, const Field_Element& gfe) {
			return os << gfe.poly_value_;
		}

	


        //check if the two Field_Element equals
        inline bool Field_Element::operator == (const Field_Element& gfe) const
        {
            return ((field_ == gfe.field_) && (poly_value_ == gfe.poly_value_));
        }
        //check if value equals to the poly_value_ of an Field_Element
        inline bool Field_Element::operator == (const field_symbol& v) const
        {
            return (poly_value_ == v);
        }
        //check if the two Field_Element are different
        inline bool Field_Element::operator != (const Field_Element& gfe) const
        {
            return ((field_ != gfe.field_) || (poly_value_ != gfe.poly_value_));
        }
        //check if value is different to the poly_value_ of an Field_Element
        inline bool Field_Element::operator != (const field_symbol& v) const {
            return (poly_value_ != v);
        }
        //check if this is smaller then gfe by comparing there poly_value_
        inline bool Field_Element::operator < (const Field_Element& gfe)
        {
            return (poly_value_ < gfe.poly_value_);
        }
        //check if this poly_value_ is smaller then v
        inline bool Field_Element::operator < (const field_symbol& v)
        {
            return (poly_value_ < v);
        }
        //check if this is bigger then gfe by comparing there poly_value_
        inline bool Field_Element::operator > (const Field_Element& gfe)
        {
            return (poly_value_ > gfe.poly_value_);
        }
        //check if this poly_value_ is bigger then v
        inline bool Field_Element::operator > (const field_symbol& v)
        {
            return (poly_value_ > v);
        }
        //function that returns the index of the Field_Element
        inline field_symbol Field_Element::index() const
        {
            //log(poly_value_)
            return field_.index(poly_value_);
        }
        //function that returns the poly_value_ (coefficient) of this Field_Element
        inline field_symbol Field_Element::poly() const
        {
            return poly_value_;
        }

        //function that returns a reference to the poly_value_ (coefficient) of this Field_Element
        inline field_symbol& Field_Element::poly()
        {
            return poly_value_;
        }

        //function that returns a refernce to the field of ths Field_Element
        inline const Field& Field_Element::galois_field() const
        {
            return field_;
        }

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
	this->poly_value_ = this->field_.add(poly_value_, gfe.poly_value_);
	return *this;
}

inline Field_Element& Field_Element::operator += (const field_symbol& value) {
	this->poly_value_ = this->field_.add(poly_value_, value);
	return *this;
}

inline Field_Element& Field_Element::operator -= (const Field_Element& gfe) {
	this->poly_value_ = this->field_.sub(poly_value_, gfe.poly_value_);
	return *this;
}

inline Field_Element& Field_Element::operator -= (const field_symbol& value) {
	this->poly_value_ = this->field_.sub(poly_value_, value);
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

inline Field_Element& Field_Element::operator ^= (const int& n) {
	this->poly_value_ = field_.exp(poly_value_, n);
	return *this;
}

inline field_symbol Field_Element::inverse() const {
	return field_.inverse(poly_value_);
}

inline void Field_Element::normalize() {
	//assign the modulo to keep in range of field size
	 poly_value_ &= field_.size();
}



	}
}
