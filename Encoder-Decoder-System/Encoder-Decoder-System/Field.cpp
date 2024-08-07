#include "Field.h"

namespace wd_codec{
    namespace galois{

        Field::Field(const unsigned int  pwr, const std::size_t primpoly_deg, const unsigned int* primitive_poly)
            :power_(pwr),
             prim_poly_deg_(primpoly_deg),
             //to reach last number in gf(a^power)
             field_size_((1>>power_) -1)
        {
            alpha_to_ = new field_symbol[field_size_ + 1];
            index_of_ = new field_symbol[field_size_ + 1];

            //TODO: check why mult with 3 is not enouph
            static const std::size_t buffer_size = (4 * (1 * field_size_) * (1 * field_size_) + 2 * (1 * field_size_) * sizeof(field_symbol));

            buffer_ = new char[buffer_size];
            std::size_t offset = 0;
            offset = create_2d_array(buffer_, (1 * field_size_), (1 * field_size_), offset, &mul_table_);
            offset = create_2d_array(buffer_, (1 * field_size_), (1 * field_size_), offset, &div_table_);
            offset = create_2d_array(buffer_, (1 * field_size_), (1 * field_size_), offset, &exp_table_);
            //TODO: check about linear_exp_table_
            offset = create_array(buffer_, (1 * field_size_) * 2, offset, &mul_inverse_);

            prim_poly_ = new unsigned int[prim_poly_deg_ + 1];
            for (unsigned int i = 0; i < prim_poly_deg_ + 1; i++)
            {
                prim_poly_[i] = primitive_poly[i];
            }

            //Create a unique hash value for a given primitive polynomial to provide optimization for comparing two fields.            prim_poly_hash_ = 0xAAAAAAAA;  //10101010101010b good for hashing
            for (std::size_t i = 0; i < (prim_poly_deg_ + 1); ++i)
            {
                prim_poly_hash_ += ((i & 1) == 0) ? ((prim_poly_hash_ << 7) ^ primitive_poly[i] * (prim_poly_hash_ >> 3)) :
                    (~((prim_poly_hash_ << 11) + (primitive_poly[i] ^ (prim_poly_hash_ >> 5))));
            }

            generate_field(prim_poly_);

        }

        Field::~Field() {
            //TODO: check about the: #if !defined(NO_GFLUT)
            if (!alpha_to_)    { delete[] alpha_to_; alpha_to_ = 0; }
            if (!index_of_)    { delete[] index_of_; index_of_ = 0; }
            if (!mul_inverse_) { delete[] mul_inverse_; mul_inverse_ = 0; }
            if (!prim_poly_)   { delete[] prim_poly_; prim_poly_ = 0; }
            if (!mul_table_)   { delete[] mul_table_; mul_table_ = 0; }
            if (!div_table_)   { delete[] div_table_; div_table_ = 0; }
            if (!exp_table_)   { delete[] exp_table_; exp_table_ = 0; }
            if (!buffer_)      { delete[] buffer_; buffer_ = 0; }
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
            // e + b = 0 = field_size_(because the field is cyclic
            //field_size_ - log(val)
            return alpha_to_[normalize(field_size_ - index_of_[val])];
        }

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
	}
}

