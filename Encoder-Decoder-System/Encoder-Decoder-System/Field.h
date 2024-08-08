#pragma once
#include <string>
#include <iostream>
#include <cstddef>
#include <vector>

namespace wd_codec {

	namespace galois {

		typedef int field_symbol;

		class Field
		{
		public:
			Field(const unsigned int  pwr, const std::size_t primpoly_deg, const unsigned int* primitive_poly);
			Field();
			~Field();
             // convert a field element to its corresponding exponent in terms of a primitive element
            inline field_symbol index(const field_symbol value) const {
                return index_of_[value];
            }
            //returns the field element corresponding to a given exponent using a precomputed antilog table.
            inline field_symbol alpha(const field_symbol value) const {
                return alpha_to_[value];
            }
            //get size of the Galois Field.
            inline unsigned int size() const {
                return field_size_;
            }
            //returns the power m used to define the field size 2^m
            inline unsigned int pwr() const {
                return power_;
            }
            //returns a mask value used for bitwise operations
            inline unsigned int mask() const {
                return field_size_;
            }
            //addition in a Galois Field
            inline field_symbol add(const field_symbol& a, const field_symbol& b) const {
                return (a ^ b);
            }
            //subtraction of two field elements
            inline field_symbol sub(const field_symbol& a, const field_symbol& b) const {
                return (a ^ b);
            }
            inline field_symbol normalize(field_symbol x) const;

            inline field_symbol mul(const field_symbol& a, const field_symbol& b) const
            {
               #if !defined(NO_GFLUT)
                   return mul_table_[a][b];
               #else
                if ((a == 0) || (b == 0))
                    return 0;
                else
                    return alpha_to_[normalize(index_of_[a] + index_of_[b])];
              #endif
            }

            inline field_symbol div(const field_symbol& a, const field_symbol& b) const
            {
              #if !defined(NO_GFLUT)
                return div_table_[a][b];
              #else
                if ((a == 0) || (b == 0))
                    return 0;
                else
                    return alpha_to_[normalize(index_of_[a] - index_of_[b] + field_size_)];
              #endif
            }

            inline field_symbol exp(const field_symbol& a, int n) const
            {
               #if !defined(NO_GFLUT)
                if (n >= 0)
                    return exp_table_[a][n & field_size_];
                else
                {
                    while (n < 0) n += field_size_;

                    return (n ? exp_table_[a][n] : 1);
                }
               #else
                if (a != 0)
                {
                    if (n < 0)
                    {
                        while (n < 0) n += field_size_;
                        return (n ? alpha_to_[normalize(index_of_[a] * n)] : 1);
                    }
                    else if (n)
                        return alpha_to_[normalize(index_of_[a] * static_cast<field_symbol>(n))];
                    else
                        return 1;
                }
                else
                    return 0;
            #endif
            }

            inline field_symbol inverse(const field_symbol& val) const {
                //todo #if
                return mul_inverse_[val];
            }

            //accesses the coefficients of the primitive polynomial at a specified index.prim_poly_term(0) returns 1 (coefficient of x^0)
            inline unsigned int prim_poly_term(const unsigned int index) const {
                return prim_poly_[index];
            }
            friend std::ostream& operator << (std::ostream& os, const Field& gf);
            inline bool operator==(const Field& field) const;
            inline bool operator!=(const Field& field) const;

		private:
			//finite field parameters:
		    unsigned int   power_;
			std::size_t    prim_poly_deg_;
			unsigned int   field_size_;
			unsigned int   prim_poly_hash_;
			unsigned int*  prim_poly_;
			//lookup tables:
			field_symbol* alpha_to_;  //mapping exponents to field elements
			field_symbol* index_of_;  //mapping field elements to exponents
			field_symbol* mul_inverse_;
			field_symbol** mul_table_;
			field_symbol** div_table_;
			field_symbol** exp_table_; //exp_table_[i][j] = alpha^(j*i)
			field_symbol** linear_exp_table_;//for complex operations that involve linear combinations of field elements 
			char* buffer_;
            
            
            void         generate_field(const unsigned int* prim_poly_);
            field_symbol gen_mul(const field_symbol& a, const field_symbol& b) const;
            field_symbol gen_div(const field_symbol& a, const field_symbol& b) const;
            field_symbol gen_exp(const field_symbol& a, const std::size_t& n) const;
            field_symbol gen_inverse(const field_symbol& val) const;

            std::size_t create_array(char buffer_[],
                const std::size_t& length,
                const std::size_t offset,
                field_symbol** array);

            std::size_t create_2d_array(char buffer_[],
                std::size_t row_cnt, std::size_t col_cnt,
                const std::size_t offset,
                field_symbol*** array);


		};
        //define basic primitive polynomials
        //start from 4 becuase the min possible m for gf(2^m) is 2
        /* 1x^0 + 1x^1 + 0x^2 + 1x^3 */
        const unsigned int primitive_polynomial00[] = { 1, 1, 0, 1 };
        const unsigned int primitive_polynomial_size00 = 4;

        /* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 1x^4*/
        const unsigned int primitive_polynomial01[] = { 1, 1, 0, 0, 1 };
        const unsigned int primitive_polynomial_size01 = 5;

        /* 1x^0 + 0x^1 + 1x^2 + 0x^3 + 0x^4 + 1x^5 */
        const unsigned int primitive_polynomial02[] = { 1, 0, 1, 0, 0, 1 };
        const unsigned int primitive_polynomial_size02 = 6;

        /* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 0x^4 + 0x^5 + 1x^6 */
        const unsigned int primitive_polynomial03[] = { 1, 1, 0, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size03 = 7;

        /* 1x^0 + 0x^1 + 0x^2 + 1x^3 + 0x^4 + 0x^5 + 0x^6 + 1x^7 */
        const unsigned int primitive_polynomial04[] = { 1, 0, 0, 1, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size04 = 8;

        /* 1x^0 + 0x^1 + 1x^2 + 1x^3 + 1x^4 + 0x^5 + 0x^6 + 0x^7 + 1x^8 */
        const unsigned int primitive_polynomial05[] = { 1, 0, 1, 1, 1, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size05 = 9;

        /* 1x^0 + 1x^1 + 1x^2 + 0x^3 + 0x^4 + 0x^5 + 0x^6 + 1x^7 + 1x^8 */
        const unsigned int primitive_polynomial06[] = { 1, 1, 1, 0, 0, 0, 0, 1, 1 };
        const unsigned int primitive_polynomial_size06 = 9;

        /* 1x^0 + 0x^1 + 0x^2 + 0x^3 + 1x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 1x^9 */
        const unsigned int primitive_polynomial07[] = { 1, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size07 = 10;

        /* 1x^0 + 0x^1 + 0x^2 + 1x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 1x^10 */
        const unsigned int primitive_polynomial08[] = { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size08 = 11;

        /* 1x^0 + 0x^1 + 1x^2 + 0x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 1x^11 */
        const unsigned int primitive_polynomial09[] = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size09 = 12;

        /* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 1x^4 + 0x^5 + 1x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 1x^12 */
        const unsigned int primitive_polynomial10[] = { 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size10 = 13;

        /* 1x^0 + 1x^1 + 0x^2 + 1x^3 + 1x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 0x^12 + 1x^13 */
        const unsigned int primitive_polynomial11[] = { 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size11 = 14;

        /* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 0x^4 + 0x^5 + 1x^6 + 0x^7 + 0x^8 + 0x^9 + 1x^10 + 0x^11 + 0x^12 + 0x^13 + 1x^14 */
        const unsigned int primitive_polynomial12[] = { 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size12 = 15;

        /* 1x^0 + 1x^1 + 0x^2 + 0x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 0x^12 + 0x^13 + 0x^14 + 1x^15 */
        const unsigned int primitive_polynomial13[] = { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size13 = 16;

        /* 1x^0 + 1x^1 + 0x^2 + 1x^3 + 0x^4 + 0x^5 + 0x^6 + 0x^7 + 0x^8 + 0x^9 + 0x^10 + 0x^11 + 1x^12 + 0x^13 + 0x^14 + 0x^15 + 1x^16 */
        const unsigned int primitive_polynomial14[] = { 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 };
        const unsigned int primitive_polynomial_size14 = 17;

	}
}

