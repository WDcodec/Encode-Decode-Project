#pragma once
#include <cassert>
#include <iostream>
#include <vector>

#include "Field.h"
#include "Polynomial.h"
#include "Field_Element.h"
#include "Block.h"
namespace wd_codec
{

    namespace reed_solomon
    {
        // declare the reed slomon paramters as specifying constant value (non-type arguments)
        template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
        class Encoder
        {
        public:
            typedef Block<code_length, fec_length> block_type;

            Encoder(const galois::Field& gfield, const galois::Polynomial& generator)
                : encoder_valid(code_length == gfield.size()),
                field_(gfield),
                generator_(generator)
            {}

            ~Encoder()
            {}

            bool encode(block_type& rsblock) const {
                return true;
            }

            bool encode(block_type& rsblock, const galois::Polynomial& data) const {
                const galois::field_symbol  mask = field_.mask();
                // iterate the data polynomial and add it to the rsblock data
                for (std::size_t i = 0; i < data_length; i++) {
                    //TODO: check if the cast is necessary
                    rsblock[i] = data[i] & mask;
                }
                return encode(rsblock);
            }

        private:
            const bool encoder_valid;
            const galois::Field& field_;
            const galois::Polynomial generator_;

            //compute the P(x)*x^(n-k) for making place to the redundancy 
            inline const galois::Polynomial& posit_data_in_codeword(const block_type& rsblock) {
                galois::Polynomial data(field_, code_length);
                for (std::size_t i = data_length - 1; i >= fec_length; i--) {
                    data[i] = rsblock[data_length - 1 - i];
                }
                return data;
            }

        };
    }
}