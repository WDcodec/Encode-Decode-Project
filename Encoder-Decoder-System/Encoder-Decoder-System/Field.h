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
			Field(const Field& field) {};
			~Field();
            //FOR IMPLEMENTATION
            inline field_symbol index(const field_symbol value) const;
            inline field_symbol alpha(const field_symbol value) const;
            inline unsigned int size() const;
            inline unsigned int pwr() const;
            inline unsigned int mask() const;
            inline field_symbol add(const field_symbol& a, const field_symbol& b) const;
            inline field_symbol sub(const field_symbol& a, const field_symbol& b) const;
            inline field_symbol normalize(field_symbol x) const;
            inline field_symbol mul(const field_symbol& a, const field_symbol& b) const;
            inline field_symbol div(const field_symbol& a, const field_symbol& b) const;
            inline field_symbol exp(const field_symbol& a, int n) const;
            inline field_symbol inverse(const field_symbol& val) const;
            inline unsigned int prim_poly_term(const unsigned int index) const;
            friend std::ostream& operator << (std::ostream& os, const Field& gf);



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
            
            
            
            
            //FOR IMPLEMENTATION
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

