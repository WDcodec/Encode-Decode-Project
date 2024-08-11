#include "Polynomial.h"
namespace wd_codec
{

    namespace galois
    {
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