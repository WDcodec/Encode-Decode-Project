#include "Field.h"
#include "Logger.h"

namespace wd_codec {
    namespace galois {

        const   field_symbol GFERROR = -1;

         Field::Field(const unsigned int  pwr, const std::size_t primpoly_deg, const unsigned int* primitive_poly)
            :power_(pwr),
            prim_poly_deg_(primpoly_deg),
            //to reach last number in gf(a^power)
            field_size_((1 << power_) - 1)
           {
            wd_codec::Logger::log(wd_codec::INFO, " Start generate field.");
            alpha_to_ = new field_symbol[field_size_ + 1];
            index_of_ = new field_symbol[field_size_ + 1];

            //TODO: check why mult with 3 is not enouph
            static const std::size_t buffer_size = ((4 * (field_size_ + 1) * (field_size_ + 1)) + ((field_size_ + 1) * 2)) * sizeof(field_symbol);

            buffer_ = new char[buffer_size];
            std::size_t offset = 0;
            offset = create_2d_array(buffer_, (field_size_ + 1), (field_size_ + 1), offset, &mul_table_);
            offset = create_2d_array(buffer_, (field_size_ + 1), (field_size_ + 1), offset, &div_table_);
            offset = create_2d_array(buffer_, (field_size_ + 1), (field_size_ + 1), offset, &exp_table_);
            //TODO: check about linear_exp_table_
            offset = create_array(buffer_, (field_size_ + 1) * 2, offset, &mul_inverse_);

            prim_poly_ = new unsigned int[prim_poly_deg_ + 1];
            for (unsigned int i = 0; i < prim_poly_deg_ + 1; i++)
            {
                prim_poly_[i] = primitive_poly[i];
            }
            prim_poly_hash_ = 0xAAAAAAAA;
            //Create a unique hash value for a given primitive polynomial to provide optimization for comparing two fields.            prim_poly_hash_ = 0xAAAAAAAA;  //10101010101010b good for hashing
            for (std::size_t i = 0; i < (prim_poly_deg_ + 1); i++)
            {
                prim_poly_hash_ += ((i & 1) == 0) ? ((prim_poly_hash_ << 7) ^ primitive_poly[i] * (prim_poly_hash_ >> 3)) :
                    (~((prim_poly_hash_ << 11) + (primitive_poly[i] ^ (prim_poly_hash_ >> 5))));
            }

            generate_field(prim_poly_);

        }

        Field::~Field() {
            //TODO: check about the: #if !defined(NO_GFLUT)
            if (!alpha_to_) { delete[] alpha_to_; alpha_to_ = 0; }
            if (!index_of_) { delete[] index_of_; index_of_ = 0; }
            if (!mul_inverse_) { delete[] mul_inverse_; mul_inverse_ = 0; }
            if (!prim_poly_) { delete[] prim_poly_; prim_poly_ = 0; }
            if (!mul_table_) { delete[] mul_table_; mul_table_ = 0; }
            if (!div_table_) { delete[] div_table_; div_table_ = 0; }
            if (!exp_table_) { delete[] exp_table_; exp_table_ = 0; }
            if (!buffer_) { delete[] buffer_; buffer_ = 0; }
        }

        bool Field::operator==(const Field& field) const {
            return this->power_ == field.power_ && this->prim_poly_hash_ == field.prim_poly_hash_;
        }

        bool Field::operator!=(const Field& field) const {
            return !operator==(field);
        }

        std::ostream& operator << (std::ostream& os, const Field& field) {
            for (int i = 0; i < field.field_size_; i++) {
                os << i << ": " << "alpha[i]= " << field.alpha_to_[i] << " index[i]= " << field.index_of_[i] << "\n";
            }
            return os;
        }

        field_symbol Field::gen_mul(const field_symbol& a, const field_symbol& b) const
        {
            if ((a == 0) || (b == 0)) // if either operand is zero, the result is zero.
                return 0;
            else
                // log(a) + log(b), normalize, and look up the corresponding field element.
                return alpha_to_[normalize(index_of_[a] + index_of_[b])];
        }


        field_symbol Field::gen_div(const field_symbol& a, const field_symbol& b) const
        {
            if ((a == 0) || (b == 0)) {// if the numerator or denominator is zero, the result is zero or error(which represnt in the table as zero)
                return 0;
            }
            else
            {
                // log(a) - log(b) + (field_size_ - 1) (to ensure non-negative),
                // normalize, and look up the corresponding field element.
                return alpha_to_[normalize(index_of_[a] - index_of_[b] + (field_size_ - 1))];
            }
        }
         field_symbol Field::gen_exp(const field_symbol& a, const std::size_t& n) const
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
         field_symbol Field::gen_inverse(const field_symbol& val) const
        {
            // e = log(val)
            // we want to find b so:
            // (a^e)*(a^b) = 1 :   = a^(e+b) = 1
            // e + b = 0 = field_size_(because the field is cyclic)
            //b = field_size_ - log(val)
            return alpha_to_[normalize(field_size_ - index_of_[val])];
        }

        //A function that normalizes the values that deviate from the field value range.
         field_symbol Field::normalize(field_symbol x) const {
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

         void Field::generate_field(const unsigned int* prim_poly)
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
                //a^(i+1)
                mask <<= 1;
            }

            index_of_[alpha_to_[power_]] = power_;

            mask >>= 1;

            for (field_symbol i = power_ + 1; i < static_cast<field_symbol>(field_size_); ++i)
            {
                if (alpha_to_[i - 1] >= mask)//alpha_to_[i - 1]<<1 will be out of the field range so it needs to be "normalizes"
                    alpha_to_[i] = alpha_to_[power_] ^ ((alpha_to_[i - 1] ^ mask) << 1);
                else
                    alpha_to_[i] = alpha_to_[i - 1] << 1;
                //index_of_[alpha_to_[i]] = i = log(alpha_to_[i])
                index_of_[alpha_to_[i]] = i;
            }
            //log of nothig is zero!
            index_of_[0] = GFERROR;
            //becouse the field is cyclic
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

            //TODO: check about the linear exp table
            for (field_symbol i = 0; i < static_cast<field_symbol>(field_size_ + 1); ++i)
            {
                mul_inverse_[i] = gen_inverse(i);
                mul_inverse_[i + (field_size_ + 1)] = mul_inverse_[i];
            }

#endif
        }
        std::size_t Field::create_array(char buffer[], const std::size_t& length, const std::size_t offset, field_symbol** array)
        {
            const std::size_t row_size = length * sizeof(field_symbol);
            (*array) = new(buffer + offset)field_symbol[length];
            return row_size + offset;
        }

        std::size_t Field::create_2d_array(char buffer[], std::size_t row_cnt, std::size_t col_cnt, const std::size_t offset, field_symbol*** array)
        {
            const std::size_t row_size = col_cnt * sizeof(field_symbol);
            char* buffer__offset = buffer + offset;
            (*array) = new field_symbol * [row_cnt];
            for (std::size_t i = 0; i < row_cnt; ++i)
            {
                (*array)[i] = new(buffer__offset + (i * row_size))field_symbol[col_cnt];
            }
            return (row_cnt * row_size) + offset;
        }
    }
}


