#include "Field.h"
#include "Field.h"
namespace wd_codec {

    namespace galois {

        typedef int field_symbol;

        inline field_symbol Field::gen_mul(const field_symbol& a, const field_symbol& b) const
        {
            if ((a == 0) || (b == 0)) // if either operand is zero, the result is zero.
                return 0;
            else
                // log(a) + log(b), normalize, and look up the corresponding field element.
                return alpha_to_[normalize(index_of_[a] + index_of_[b])];
        }
        inline field_symbol Field::gen_div(const field_symbol& a, const field_symbol& b) const
        {
            if (b == 0)  // check division by zero 
            {
                throw std::domain_error("Division by zero in Galois Field.");
            }
            if (a == 0)  // if the numerator is zero, the result is zero
            {
                return 0;
            }
            else
            {
                // log(a) - log(b) + (field_size_ - 1) (to ensure non-negative),
                // normalize, and look up the corresponding field element.
                return alpha_to_[normalize(index_of_[a] - index_of_[b] + (field_size_ - 1))];
            }
        }
        inline field_symbol Field::gen_exp(const field_symbol& a, const std::size_t& n) const
        {
            if (a != 0)
            {   // if exponent is zero, return 1 (a^0 = 1)
                return ((n == 0)  // log(a)*n
                    ? 1 : alpha_to_[normalize(index_of_[a] * static_cast<field_symbol>(n))]);
            }
            else  // if a is zero
            {
                return 0;  // 0^x = 0 for any x
            }
        }
        // computes the multiplicative inverse of a field element 'val'
        inline field_symbol Field::gen_inverse(const field_symbol& val) const
        {
            // e = log(val)
            // we want to find b so:
            // (a^e)*(a^b) = 1 :   = a^(e+b) = 1
            // e + b = 0 = field_size_(because the field is cyclic
            //field_size_ - log(val)
            return alpha_to_[normalize(field_size_ - index_of_[val])];
        }

        // normalizes the values that deviate from the field value range.
        inline field_symbol Field::normalize(field_symbol x) const {
            while (x < 0) {
                x += static_cast<field_symbol>(field_size_);
            }
            while (x > static_cast<field_symbol>(field_size_)) {
                x -= static_cast<field_symbol>(field_size_);
                //optimization of the typical normalization(using mod) to addition of div and mod
                x = (x >> power_) + (x & field_size_);
            }
            return x;
        }
    }
}