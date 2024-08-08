#pragma once

#include "Field.h"

namespace wd_codec {
	namespace galois {

		class Field_Element
		{
		public: 
			Field_Element(const Field& field, const field_symbol& value = 0);
			Field_Element(const Field_Element& element);
			Field_Element(const Field& gfield);
			~Field_Element() {}; 

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

		private:
			const Field& field_;
			//the value of the coafficient
			field_symbol poly_value_;

		};
	}
}

