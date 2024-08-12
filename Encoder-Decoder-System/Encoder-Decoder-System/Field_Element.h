#pragma once

#include "Field.h"

namespace wd_codec {
	namespace galois {

		class Field_Element
		{
		public: 


			Field_Element(const Field& field, const field_symbol& value):field_(const_cast<Field&>(field)),poly_value_(value){}//ctor
			Field_Element(const Field& gfield):field_(gfield),poly_value_(-1){}//ctor
			Field_Element(const Field_Element& element) :field_(const_cast<Field&>(element.field_)), poly_value_(element.poly_value_) {};//copy ctor
			~Field_Element() {};//dtor 

			 Field_Element& operator = (const Field_Element& gfe);
			 Field_Element& operator = (const field_symbol& v);
			 Field_Element& operator += (const Field_Element& gfe);
			 Field_Element& operator += (const field_symbol& v);
			 Field_Element& operator -= (const Field_Element& gfe);
			 Field_Element& operator -= (const field_symbol& v);
			 Field_Element& operator *= (const Field_Element& gfe);
			 Field_Element& operator *= (const field_symbol& v);
			 Field_Element& operator /= (const Field_Element& gfe);
			 Field_Element& operator /= (const field_symbol& v);
			 Field_Element& operator ^= (const int& n);
			 bool operator == (const Field_Element& gfe) const;
			 bool operator == (const field_symbol& v) const;
			 bool operator != (const Field_Element& gfe) const;
			 bool operator != (const field_symbol& v) const;
			 bool operator < (const Field_Element& gfe);
			 bool operator < (const field_symbol& v);
			 bool operator > (const Field_Element& gfe);
			 bool operator > (const field_symbol& v);
			 field_symbol index() const;
			 field_symbol poly() const;
			 field_symbol& poly();
			 const Field& galois_field() const;
			 field_symbol inverse() const;
			 void normalize();
			friend std::ostream& operator << (std::ostream& os, const Field_Element& gfe);

		private:
			const Field& field_;
			//the value of the coefficient
			field_symbol poly_value_;

		};

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

		inline Field_Element operator / (const Field_Element& a, const Field_Element& b)
		{
			Field_Element result = a;
			result /= b;
			return result;
		}

		inline Field_Element operator ^ (const Field_Element& a, const int& b) {
			Field_Element result = a;
			result ^= b;
			return result;
		}

		
	}
}

