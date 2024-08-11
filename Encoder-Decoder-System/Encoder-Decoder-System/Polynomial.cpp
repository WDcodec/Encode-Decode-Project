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
                while ((begin != it) && (*(--it) == 0)) {
                    counter++;
                }
                //delete the trailing zeros.
                polynomial.poly_.resize(poly_size - counter);
            }
        }

        // function that check whether a plynomial is monic or not.
        bool Polynomial::monic() const {
            return poly_[poly_.size()-1] == static_cast<galois::field_symbol>(1);
        }

        // function that claculate the derivative of polynomials
        Polynomial Polynomial::derivative() const {
            // In finite fields, the derivative of a polynomial is computed using modulo arithmetic.
            // we multiply each coefficient by its exponent modulo the field's characteristic.
            // In GF(2), this means each coefficient is multiplied by 0 or 1:

            //f(x) = x ^ n 
            //f'(x) = (n%2) * x ^ (n-1) where n is 0/1
            if (field_.size() > 1) { 
                Polynomial deriv(field_, deg());
                const std::size_t upper_bound = poly_.size();
                //i+=2 becuase poly_[i + 1] * 0 = 0.
                //poly_[i + 1] * 1 = poly_[i + 1];
                for (std::size_t i = 0; i < upper_bound; i += 2)
                {
                    deriv.poly_[i] = poly_[i + 1];
                }

                simplify(deriv);
                return deriv;

            }

            return Polynomial(field_, 0);
        }


    }
}