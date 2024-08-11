#include "Polynomial.h"

namespace wd_codec
{

    namespace galois
    {
        inline Polynomial& Polynomial::operator <<= (const unsigned int& n) {
            //to shift  right the polynomial n places, nX0 in begin
            size_t  init_size= poly_.size();
            if (init_size > 0) {
                poly_.resize(init_size + n,Field_Element(field_,0));
                for (size_t i = init_size - 1; static_cast<int>(i) >= 0; --i)//move 
                {
                    poly_[i + n] = poly_[i];
                }
                for (unsigned int i = 0; i < n; ++i)//fill 0
                {
                    poly_[i] = 0;
                }
            }
            return *this;
        

        }
        inline Polynomial& Polynomial::operator >>= (const unsigned int& n) {
            // to shift  left  the polynomial n  places, make it smaller size-n
            if (n <= poly_.size())
            {
                for (unsigned int i = 0; i <= deg() - n; ++i)
                {
                    poly_[i] = poly_[i + n];
                }

                poly_.resize(poly_.size() - n, Field_Element(field_, 0));
            }
            else if (static_cast<int>(n) >= (deg() + 1))
            {
                poly_.resize(0, Field_Element(field_, 0));
            }

            return *this;
        }
    

        Polynomial operator + (const Polynomial& a, const Polynomial& b);
        Polynomial operator + (const Polynomial& a, const Field_Element& b);
        Polynomial operator + (const Field_Element& a, const Polynomial& b);
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



        inline Polynomial generate_X(const Field& gfield) {//create base polynomail
            const Field_Element xgfe[2] = {
                                        Field_Element(gfield, 0),
                                        Field_Element(gfield, 1)
            };
            Polynomial X_(gfield, 1, xgfe);//todo fix this creation
            return X_;
        }
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
       inline Polynomial operator ^ (const Polynomial& a, const int& n) {
           Polynomial result = a;
           result ^= n;
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
    }
}