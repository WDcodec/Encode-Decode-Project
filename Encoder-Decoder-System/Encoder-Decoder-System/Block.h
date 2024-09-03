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



            //template <typename T, std::size_t code_length, std::size_t fec_length, std::size_t stack_size>
            //inline void copy(const Block<code_length, fec_length> src_block_stack[stack_size],
            //    T dest_data[])
            //{
            //    const std::size_t data_length = code_length - fec_length;

            //    for (std::size_t i = 0; i < stack_size; ++i)
            //    {
            //        for (std::size_t j = 0; j < data_length; ++j, ++dest_data)
            //        {
            //            (*dest_data) = static_cast<T>(src_block_stack[i][j]);
            //        }
            //    }
            //}
            //template <std::size_t code_length, std::size_t fec_length>
            //inline void copy(const block<code_length, fec_length>& src_block, block<code_length, fec_length>& dest_block)
            //{
            //    for (std::size_t index = 0; index < code_length; ++index)
            //    {
            //        dest_block.data[index] = src_block.data[index];
            //    }
            //}

           /* template <typename T, std::size_t code_length, std::size_t fec_length>
            inline void copy(const T src_data[], block<code_length, fec_length>& dest_block)
            {
                for (std::size_t index = 0; index < (code_length - fec_length); ++index, ++src_data)
                {
                    dest_block.data[index] = static_cast<typename block<code_length, fec_length>::symbol_type>(*src_data);
                }
            }*/

            //template <typename T, std::size_t code_length, std::size_t fec_length>
            //inline void copy(const T src_data[],
            //    const std::size_t& src_length,
            //    Block<code_length, fec_length>& dest_block)
            //{
            //    for (std::size_t index = 0; index < src_length; ++index, ++src_data)
            //    {
            //        dest_block.data[index] = static_cast<typename Block<code_length, fec_length>::symbol_type>(*src_data);
            //    }
            //}
      

            //template <std::size_t code_length, std::size_t fec_length, std::size_t stack_size>
            //inline void copy(const block<code_length, fec_length>  src_block_stack[stack_size],
            //    block<code_length, fec_length> dest_block_stack[stack_size])
            //{
            //    for (std::size_t row = 0; row < stack_size; ++row)
            //    {
            //        copy(src_block_stack[row], dest_block_stack[row]);
            //    }
            //}

     /*       template <typename T, std::size_t code_length, std::size_t fec_length, std::size_t stack_size>
            inline bool copy(const T src_data[],
                const std::size_t src_length,
                block<code_length, fec_length> dest_block_stack[stack_size])
            {
                const std::size_t data_length = code_length - fec_length;

                if (src_length > (stack_size * data_length))
                {
                    return false;
                }

                const std::size_t row_count = src_length / data_length;

                for (std::size_t row = 0; row < row_count; ++row, src_data += data_length)
                {
                    copy(src_data, dest_block_stack[row]);
                }

                if ((src_length % data_length) != 0)
                {
                    copy(src_data, src_length % data_length, dest_block_stack[row_count]);
                }

                return true;
            }*/
            template <typename T, std::size_t block_length>
            struct data_block
            {
            public:

                typedef T value_type;

                T& operator[](const std::size_t index) { return data[index]; }
                const T& operator[](const std::size_t index) const { return data[index]; }

                T* begin() { return data; }
                const T* begin() const { return data; }

                T* end() { return data + block_length; }
                const T* end()   const { return data + block_length; }

                void clear(T value = 0)
                {
                    for (std::size_t i = 0; i < block_length; ++i)
                    {
                        data[i] = value;
                    }
                }

            private:

                T data[block_length];
            };

        /*    template <typename T, std::size_t block_length>
            inline void copy(const data_block<T, block_length>& src_block, data_block<T, block_length>& dest_block)
            {
                for (std::size_t index = 0; index < block_length; ++index)
                {
                    dest_block[index] = src_block[index];
                }
            }*/

         /*   template <typename T, std::size_t block_length, std::size_t stack_size>
            inline void copy(const data_block<T, block_length>  src_block_stack[stack_size],
                data_block<T, block_length> dest_block_stack[stack_size])
            {
                for (std::size_t row = 0; row < stack_size; ++row)
                {
                    copy(src_block_stack[row], dest_block_stack[row]);
                }
            }*/
        };
        template <typename T, std::size_t code_length, std::size_t fec_length>
        inline void copy(const T src_data[],
            const std::size_t& src_length,
            Block<code_length, fec_length>& dest_block)
        {
            for (std::size_t index = 0; index < src_length; ++index)
            {
                dest_block.data[index] = static_cast<typename Block<code_length, fec_length>::symbol_type>(src_data[index]);
            }
        }
    }
}

