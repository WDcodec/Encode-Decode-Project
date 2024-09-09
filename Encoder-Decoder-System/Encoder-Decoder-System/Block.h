#pragma once
#include <string>
#include "Field.h"

namespace wd_codec {
    namespace reed_solomon
    {
        template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
        class Block
        {
        public:
            // Typedefs
            typedef galois::field_symbol symbol_type;
            //typedef traits::reed_solomon_triat<code_length, fec_length, data_length> trait;
            //typedef traits::symbol<code_length> symbol;
            typedef Block<code_length, fec_length, data_length> block_t;

            // Default constructor
            Block()
                : errors_detected(0),
                errors_corrected(0),
                zero_numerators(0),
                unrecoverable(false)
                //,error(e_no_error)
            {
                // traits::validate_reed_solomon_block_parameters<code_length, fec_length, data_length>();
            }

            // Constructor with data and fec strings
            Block(const std::string& _data, const std::string& _fec)
                : errors_detected(0),
                errors_corrected(0),
                zero_numerators(0),
                unrecoverable(false)
            {
                // Ensure _data and _fec sizes are correct before processing
            /*    if (_data.size() != data_length || _fec.size() != fec_length) {
                    throw std::invalid_argument("Invalid data or fec length.");
                }*/

                // Copy data into the array
                for (std::size_t i = 0; i < data_length; ++i)
                {
                    data[i] = static_cast<galois::field_symbol>(_data[i]);
                }

                for (std::size_t i = 0; i < fec_length; ++i)
                {
                    data[i + data_length] = static_cast<galois::field_symbol>(_fec[i]);
                }
            }

            galois::field_symbol& fec(int index) {
                return data[index + data_length];
            }

            // Placing operators:

            galois::field_symbol& operator[](const std::size_t index) {
                return data[index];
            }

            const galois::field_symbol& operator[](const std::size_t index) const {
                return data[index];
            }

            galois::field_symbol& operator()(const std::size_t index) {
                return data[index];
            }

            // Member variables
            std::size_t errors_detected;
            std::size_t errors_corrected;
            std::size_t zero_numerators;
            bool unrecoverable;
            galois::field_symbol data[code_length];

        };
        template <typename T, std::size_t code_length, std::size_t fec_length>
        inline void copy(const T src_data[],
            const std::size_t& src_length,
            Block<code_length, fec_length>& dest_block)
        {
            for (std::size_t index = 0; index < src_length; ++index)
            {
                dest_block.data[index] = static_cast<typename Block<code_length, fec_length>::symbol_type>(src_data[index]);
                std::cout << dest_block.data[index];
            }
        }
    }
}

