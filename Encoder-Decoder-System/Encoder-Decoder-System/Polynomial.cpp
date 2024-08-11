#include "Polynomial.h"
namespace wd_codec
{

    namespace galois
    {
        inline Polynomial::Polynomial(const Field& gfield)
            : field_(const_cast<Field&>(gfield))
        {
            poly_.clear();
            poly_.reserve(256);//set the capacity to 256
        }
        inline Polynomial::Polynomial(const Field& gfield, const unsigned int& degree)
            : field_(const_cast<Field&>(gfield))
        {
            poly_.reserve(256);//set the capacity to 256
            //resize the poly_ to degree+1 size, and peds the poly_ with  Field_Element(field_, 0)
            poly_.resize(degree + 1, Field_Element(field_, 0));
        }

        inline Polynomial::Polynomial(const Field& gfield, const unsigned int& degree, const Field_Element element[])
            : field_(const_cast<Field&>(gfield))
        {
            poly_.reserve(256);

            if (element != NULL)
            {
                /*
                   It is assumed that element is an array of field elements
                   with size/element count of degree + 1.
                */
                for (unsigned int i = 0; i <= degree; ++i)
                {
                    poly_.push_back(element[i]);
                }
            }
            else
                poly_.resize(degree + 1, Field_Element(field_, 0));
        }

        inline Polynomial::Polynomial(const Polynomial& polynomial)
            : field_(const_cast<Field&>(polynomial.field_)),
            poly_(polynomial.poly_)
        {}

        inline Polynomial::Polynomial(const Field_Element& element)
            : field_(const_cast<Field&>(element.galois_field()))
        {
            poly_.resize(1, element);
        }


        Polynomial operator + (const Polynomial& a, const Polynomial& b);
        Polynomial operator + (const Polynomial& a, const Field_Element& b);
        Polynomial operator + (const Polynomial& a, const Polynomial& b);
        Polynomial operator + (const Polynomial& a, const field_symbol& b);
        Polynomial operator + (const field_symbol& a, const Polynomial& b);
        Polynomial operator - (const Polynomial& a, const Polynomial& b);
        Polynomial operator - (const Polynomial& a, const Field_Element& b);
        Polynomial operator - (const Field_Element& a, const Polynomial& b);
        Polynomial operator - (const Polynomial& a, const field_symbol& b);
        Polynomial operator - (const field_symbol& a, const Polynomial& b);
        Polynomial operator * (const Polynomial& a, const Polynomial& b);
        Polynomial operator * (const Field_Element& a, const Polynomial& b);
        Polynomial operator * (const Polynomial& a, const Field_Element& b);
        Polynomial operator / (const Polynomial& a, const Polynomial& b);
        Polynomial operator / (const Polynomial& a, const Field_Element& b);
        Polynomial operator % (const Polynomial& a, const Polynomial& b);
        Polynomial operator % (const Polynomial& a, const unsigned int& power);
        Polynomial operator ^ (const Polynomial& a, const int& n);
        Polynomial operator <<(const Polynomial& a, const unsigned int& n);
        Polynomial operator >>(const Polynomial& a, const unsigned int& n);
        Polynomial         gcd(const Polynomial& a, const Polynomial& b);
        inline Polynomial generate_X(const Field& gfield);

    }
}