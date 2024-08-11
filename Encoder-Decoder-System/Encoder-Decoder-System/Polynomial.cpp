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


        // Function that deletes unnecessary trailing zeros.
        inline void Polynomial::simplify(Polynomial& polynomial) const {
            std::size_t poly_size = polynomial.poly_.size();
            // If the last coefficient is not zero, the polynomial is in its simplified representation.
            if ((poly_size > 0) && (polynomial.poly_.back() == 0)) {

                poly_iter it = polynomial.poly_.end();
                poly_iter begin = polynomial.poly_.begin();
                int counter = 0;

                // Iterate and count all trailing zeros, as long as there are any.
                while ((begin != it) && (*(--it) == 0)) {
                    counter++;
                }

                // Delete the trailing zeros.
                polynomial.poly_.resize(poly_size - counter);
            }
        }

        // Function that check whether a plynomial is monic or not.
        bool Polynomial::monic() const {
            return poly_[poly_.size()-1] == static_cast<galois::field_symbol>(1);
        }

        // Function that claculate the derivative of polynomials
        Polynomial Polynomial::derivative() const {
            /* In finite fields, the derivative of a polynomial is computed using modulo arithmetic.
             we multiply each coefficient by its exponent modulo the field's characteristic.
             In GF(2), this means each coefficient is multiplied by 0 or 1:
            f(x) = x ^ n 
            f'(x) = (n%2) * x ^ (n-1) where n is 0/1*/

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

        // Function that multiply two polynomials
        Polynomial& Polynomial::operator *=  (const Polynomial& polynomial) {
            if (&field_ == &(polynomial.field_)) {
                Polynomial product(field_, deg() + polynomial.deg() + 1);
                poly_iter prod_it = product.poly_.begin();
                std::size_t A_size = deg();
                std::size_t B_size = polynomial.deg();

                for (int A_i = 0; A_i < A_size; A_i++) {
                    for (int B_i = 0; B_i < B_size; B_i++) {
                        prod_it[A_i + B_i] = poly_[A_i] * polynomial.poly_[B_i];
                    }
                }
                simplify(product);
                poly_ = product.poly_;
            }
            return *this;
        }

        // Function that multiply polynomial by a scalar
        Polynomial& Polynomial::operator *=  (const Field_Element& element) {
            if (&field_ == &(element.galois_field())) {
                for (poly_iter it = this->poly_.begin(); it != this->poly_.end(); it++) {
                    *(it)*=element;
                }
            }
            return *this;
        }
        // Function that implements polynomial division using the long division algorithm
        inline void Polynomial::long_division (const Polynomial& divisor, Polynomial& remainder, Polynomial& quotient)
        {
            if (
                (&field_ == &divisor.field_) &&
                (deg() >= divisor.deg()) &&
                (divisor.deg() >= 0)
                )
            {

                // Long division algorithm:
                for (int i = static_cast<int>(deg()); i >= 0; i--)
                {
                    // if the current degree matches or is within the range of the quotient polynomial
                    if (i <= static_cast<int>(quotient.deg()))
                    {
                        //q = p / d
                        // calculate the coefficient for the current term in the quotient polynomial
                        quotient[i] = remainder[remainder.deg()] / divisor[divisor.deg()];

                        // update the remainder by subtracting the product of the quotient term and divisor
                        for (int j = static_cast<int>(remainder.deg()); j > 0; --j)
                        {
                            // r = p - q * d
                            remainder[j] = remainder[j - 1] + (quotient[i] * divisor[j]);
                        }

                        // adjust the lowest degree term of the remainder
                        remainder[0] = poly_[i] + (quotient[i] * divisor[0]);
                    }
                    else
                    {
                        // shift the remainder polynomial down to align with the next term in the dividend
                        for (int j = static_cast<int>(remainder.deg()); j > 0; --j)
                        {
                            remainder[j] = remainder[j - 1];
                        }

                        // set the leading term of the remainder to the current term of the dividend polynomial
                        remainder[0] = poly_[i];
                    }
                }
                simplify(quotient);
                simplify(remainder);
            }
        }

        inline Polynomial& Polynomial::operator %= (const Polynomial& divisor) {
            if (
                (field_ == divisor.field_) &&
                (deg() >= divisor.deg()) &&
                (divisor.deg() >= 0)
                )
            {
                Polynomial quotient(field_, deg() - divisor.deg() + 1);
                Polynomial remainder(field_, divisor.deg() - 1);
                long_division(divisor, quotient, remainder);
                poly_ = remainder.poly_;
            }

            return *this;
        }

        inline Polynomial& Polynomial::operator /= (const Polynomial& divisor) {
            if (
                (field_ == divisor.field_) &&
                (deg() >= divisor.deg()) &&
                (divisor.deg() >= 0)
                )
            {
                Polynomial quotient(field_, deg() - divisor.deg() + 1);
                Polynomial remainder(field_, divisor.deg() - 1);
                long_division(divisor, quotient, remainder);
                poly_ = quotient.poly_;
            }

            return *this;
        }


        // Function that divide polynomial by a scalar
        Polynomial& Polynomial::operator/= (const Field_Element& element) {
            if (&field_ == &(element.galois_field())) {
                for (poly_iter it = this->poly_.begin(); it != this->poly_.end(); it++) {
                    *(it) /= element;
                }
            }
            return *this;
        }
    

    }
}