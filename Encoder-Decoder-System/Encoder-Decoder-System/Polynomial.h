#pragma once
#include <vector>
#include <cassert>
#include <iostream>

#include "Field.h"
#include "Field_Element.h"
#include "Polynomial.h"

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

            int deg() const {
                return poly_.size() - 1;
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

	}
}
