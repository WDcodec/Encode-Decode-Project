#pragma once

#include "Field.h"

namespace wd_codec {
	namespace galois {

		class Field_Element
		{
		public: 


			Field_Element(const Field& field, const field_symbol& value):field_(const_cast<Field&>(field)),poly_value_(value){}//ctor
			Field_Element(const Field& gfield):field_(gfield),poly_value_(-1){}//ctor
			Field_Element(const Field_Element& element) :field_(const_cast<Field&>(element.field_)), poly_value_(element.poly_value_) {}//copy ctor
			~Field_Element() {};//dtor 

			inline Field_Element& operator = (const Field_Element& gfe);
			inline Field_Element& operator = (const field_symbol& v);
			inline Field_Element& operator += (const Field_Element& gfe);
			inline Field_Element& operator += (const field_symbol& v);
			inline Field_Element& operator -= (const Field_Element& gfe);
			inline Field_Element& operator -= (const field_symbol& v);
			inline Field_Element& operator *= (const Field_Element& gfe);
			inline Field_Element& operator *= (const field_symbol& v);
			inline Field_Element& operator /= (const Field_Element& gfe);
			inline Field_Element& operator /= (const field_symbol& v);
			inline Field_Element& operator ^= (const int& n);
			inline bool operator == (const Field_Element& gfe) const;
			inline bool operator == (const field_symbol& v) const;
			inline bool operator != (const Field_Element& gfe) const;
			inline bool operator != (const field_symbol& v) const;
			inline bool operator < (const Field_Element& gfe);
			inline bool operator < (const field_symbol& v);
			inline bool operator > (const Field_Element& gfe);
			inline bool operator > (const field_symbol& v);
			inline field_symbol index() const;
			inline field_symbol poly() const;
			inline field_symbol& poly();
			inline const Field& galois_field() const;
			inline field_symbol inverse() const;
			inline void normalize();
			friend std::ostream& operator << (std::ostream& os, const Field_Element& gfe);

		//private:
			const Field& field_;
			//the value of the coefficient
			field_symbol poly_value_;

		};
	}
}

