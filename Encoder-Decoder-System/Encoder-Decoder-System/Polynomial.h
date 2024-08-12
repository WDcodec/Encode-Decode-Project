#pragma once
#include <vector>
#include <cassert>
#include <iostream>

#include "Field_Element.h"


namespace wd_codec
{

	namespace galois
	{

		class Polynomial
		{
       
        public: 
            Polynomial(const Field& gfield);
            Polynomial(const Field& gfield, const unsigned int& degree);
            Polynomial(const Field& gfield, const unsigned int& degree, const Field_Element element[]);
            Polynomial(const Polynomial& polynomial);
            Polynomial(const Field_Element& gfe);
            ~Polynomial() {}

            bool valid() const {
                return poly_.size() > 0;
            }

            inline int deg() const {
                return static_cast<int>(poly_.size()) - 1;
            }

            const Field& galois_field() const {
                return field_;
            }

            void set_degree(const unsigned int& x) {
                poly_.resize(x - 1,Field_Element(field_,0));
            }

            void simplify() {
                simplify(*this);
            }
            

            Polynomial& operator  =  (const Polynomial& polynomial);
            Polynomial& operator  =  (const Field_Element& element);
            Polynomial& operator +=  (const Polynomial& element);
            Polynomial& operator +=  (const Field_Element& element);
            Polynomial& operator -=  (const Polynomial& element);
            Polynomial& operator -=  (const Field_Element& element);
            Polynomial& operator *=  (const Polynomial& polynomial);
            Polynomial& operator *=  (const Field_Element& element);
            Polynomial& operator /=  (const Polynomial& divisor);
            Polynomial& operator /=  (const Field_Element& element);
            Polynomial& operator %=  (const Polynomial& divisor);
            Polynomial& operator %=  (const unsigned int& power);
            Polynomial& operator ^=  (const unsigned int& n);
            Polynomial& operator <<= (const unsigned int& n);
            Polynomial& operator >>= (const unsigned int& n);

            Field_Element&    operator[] (const std::size_t& term);
            Field_Element     operator() (const Field_Element& value);
            Field_Element     operator() (field_symbol                 value);

            const Field_Element& operator[](const std::size_t& term) const;
            const Field_Element  operator()(const Field_Element& value) const;
            const Field_Element  operator()(field_symbol         value) const;

            bool operator==(const Polynomial& polynomial) const;
            bool operator!=(const Polynomial& polynomial) const;

            bool monic() const;

            Polynomial derivative() const;

            void long_division(const Polynomial& divisor, Polynomial& remainder, Polynomial& quotient);

            friend std::ostream& operator << (std::ostream& os, const Polynomial& polynomial);

      private:

            typedef std::vector<Field_Element>::iterator       poly_iter;
            typedef std::vector<Field_Element>::const_iterator const_poly_iter;

            void simplify(Polynomial& polynomial) const;

			Field& field_;
			std::vector<Field_Element> poly_;

		};

		////non member functions:

		inline Polynomial operator + (const Polynomial& a, const Polynomial& b) {
			Polynomial result = a;
			result += b;
			return result;
		}
		inline Polynomial operator + (const Polynomial& a, const Field_Element& b) {
			Polynomial result = a;
			result += b;
			return result;
		}
		inline Polynomial operator + (const Field_Element& a, const Polynomial& b) {
			Polynomial result = b;
			result += a;
			return result;
		}
		inline Polynomial operator + (const Polynomial& a, const field_symbol& b) {
			return a + Field_Element(a.galois_field(), b);
		}
		inline Polynomial operator + (const field_symbol& a, const Polynomial& b) {
			return b + Field_Element(b.galois_field(), a);
		}
		inline Polynomial operator - (const Polynomial& a, const Polynomial& b) {
			return a - b;
		}
		inline Polynomial operator - (const Polynomial& a, const Field_Element& b) {
			Polynomial result = a;
			result -= b;
			return result;
		}
		inline Polynomial operator - (const Field_Element& a, const Polynomial& b) {
			Polynomial result = b;
			result -= a;
			return result;
		}
		inline Polynomial operator - (const Polynomial& a, const field_symbol& b) {
			return a - Field_Element(a.galois_field(), b);
		}
		inline Polynomial operator - (const field_symbol& a, const Polynomial& b) {
			return b - Field_Element(b.galois_field(), a);
		}
		inline Polynomial operator * (const Polynomial& a, const Polynomial& b) {
			Polynomial result = a;
			result *= b;
			return result;
		}
		inline Polynomial operator * (const Field_Element& a, const Polynomial& b) {
			Polynomial result = a;
			result *= b;
			return result;
		}
		inline Polynomial operator * (const Polynomial& a, const Field_Element& b) {
			Polynomial result = a;
			result *= b;
			return result;
		}

		inline Polynomial operator / (const Polynomial& a, const Polynomial& b) {
			Polynomial result = a;
			result /= b;
			return result;
		}

		inline Polynomial operator / (const Polynomial& a, const Field_Element& b) {
			Polynomial result = a;
			result /= b;
			return result;
		}

		inline  Polynomial operator % (const Polynomial& a, const Polynomial& b) {
			return a % b;
		}

		inline Polynomial operator % (const Polynomial& a, const unsigned int& power) {
			Polynomial result = a;
			result %= power;
			return result;
		}

		inline Polynomial operator <<(const Polynomial& a, const unsigned int& n) {
			Polynomial result = a;
			result <<= n;
			return result;
		}

		inline Polynomial operator >>(const Polynomial& a, const unsigned int& n) {
			Polynomial result = a;
			result >>= n;
			return result;
		}

		inline Polynomial operator ^ (const Polynomial& a, const int& n) {
			Polynomial result = a;
			result ^= n;
			return result;
		}

		inline Polynomial gcd(const Polynomial& a, const Polynomial& b)
		{//calculates the greatest common divisor of two polynomials ,  ensuring both polynomials same Galois Field.
			if (&a.galois_field() == &b.galois_field())
			{
				if ((!a.valid()) && (!b.valid()))//not same 
				{
					Polynomial error_polynomial(a.galois_field());
					return error_polynomial;
				}

				if (!a.valid()) return b;
				if (!b.valid()) return a;

				Polynomial x = a % b;
				Polynomial y = b;
				Polynomial z = x;

				while ((z = (y % x)).valid())
				{
					y = x;
					x = z;
				}
				return x;
			}
			else
			{
				Polynomial error_polynomial(a.galois_field());
				return error_polynomial;
			}
		}

	}
}
