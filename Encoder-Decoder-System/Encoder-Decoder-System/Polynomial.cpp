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


        // function that delete unnecessary trailing zeros.
        inline void Polynomial::simplify(Polynomial& polynomial) const {
            std::size_t poly_size = polynomial.poly_.size();
            //if the last coefficient is not zero, the poly is in it simplify representation.
            if ((poly_size > 0) && (polynomial.poly_.back() == 0)) {

                poly_iter it = polynomial.poly_.end();
                poly_iter begin = polynomial.poly_.begin();
                int counter = 0;

                //iterate and count all trailing zeros,as long as there is.
                while ((begin != it) && (*(--it) == 0)){
                    counter++;
                }
                //delete the trailing zeros.
                polynomial.poly_.resize(poly_size - counter, Field_Element(field_,0));
            }
        }


    }
}