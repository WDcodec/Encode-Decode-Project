#include "Field.h"

namespace wd_codec {

	namespace galois {

        typedef int field_symbol;

        const   field_symbol GFERROR = -1;

		//A function that normalizes the values that deviate from the field value range.
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
            // e + b = 0 = field_size_(because the field is cyclic)
            //field_size_ - log(val)
            return alpha_to_[normalize(field_size_ - index_of_[val])];
        }
        inline void Field::generate_field(const unsigned int* prim_poly)
        {
            field_symbol mask = 1;

            alpha_to_[power_] = 0;

            for (field_symbol i = 0; i < static_cast<field_symbol>(power_); ++i)
            {
                //a is the primitive element
                //alpha_to_[i] = a^i
                alpha_to_[i] = mask;
                //index_of_[a^i] = i  (index_of_[a^i] = log(a^i)
                index_of_[alpha_to_[i]] = i;
                // checks if the current coefficient in the primitive polynomial is 1 (non-zero).
                if (prim_poly[i] != 0)
                {
                    alpha_to_[power_] ^= mask;
                }

                mask <<= 1;
            }

            index_of_[alpha_to_[power_]] = power_;

            mask >>= 1;

            for (field_symbol i = power_ + 1; i < static_cast<field_symbol>(field_size_); ++i)
            {
                if (alpha_to_[i - 1] >= mask)//alpha_to_[i - 1]<<1 will be out of the field range so it needs to be "normalizes
                    alpha_to_[i] = alpha_to_[power_] ^ ((alpha_to_[i - 1] ^ mask) << 1);
                else
                    alpha_to_[i] = alpha_to_[i - 1] << 1;
                //index_of_[alpha_to_[i]] = i = log(alpha_to_[i])
                index_of_[alpha_to_[i]] = i;
            }
            //log of nothig is zero!
            index_of_[0] = GFERROR;
            alpha_to_[field_size_] = 1;

            #if !defined(NO_GFLUT)

            for (field_symbol i = 0; i < static_cast<field_symbol>(field_size_ + 1); ++i)
            {
                for (field_symbol j = 0; j < static_cast<field_symbol>(field_size_ + 1); ++j)
                {
                    mul_table_[i][j] = gen_mul(i, j);
                    div_table_[i][j] = gen_div(i, j);
                    exp_table_[i][j] = gen_exp(i, j);
                }
            }

            #ifdef LINEAR_EXP_LUT
            for (field_symbol i = 0; i < static_cast<field_symbol>(field_size_ + 1); ++i)
            {
                for (int j = 0; j < static_cast<field_symbol>(2 * field_size_); ++j)
                {
                    linear_exp_table_[i][j] = gen_exp(i, j);
                }
            }
            #endif

            for (field_symbol i = 0; i < static_cast<field_symbol>(field_size_ + 1); ++i)
            {
                mul_inverse_[i] = gen_inverse(i);
                mul_inverse_[i + (field_size_ + 1)] = mul_inverse_[i];
            }

        #endif
        }
	}
}

