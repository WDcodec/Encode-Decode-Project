

#pragma once

#include "Logger.h";
#include "Field.h";
#include "Polynomial.h"
#include "Encoder.h";
#include "Generator_polynomial.h"
#include "Fileio.h"
#include "File_Encoder.h"
#include "Error_Injection.h"
#include "Decoder.h"
#include "File_Decoder.h"
namespace wd_codec {

    /* Finite Field Parameters */
    const std::size_t field_descriptor = 8;
    const std::size_t generator_polynomial_index = 120;
    const std::size_t generator_polynomial_root_count = 32;

    /* Reed Solomon Code Parameters */
    const std::size_t code_length = 255;
    const std::size_t fec_length = 32;
    const std::size_t data_length = code_length - fec_length;

    const std::size_t stack_size = 255;

    /* Instantiate Finite Field and Generator Polynomials */

    const wd_codec::galois::Field field(field_descriptor,
        wd_codec::galois::primitive_polynomial_size06,
        wd_codec::galois::primitive_polynomial06);

    wd_codec::galois::Polynomial generator_polynomial(field);
    /* Instantiate RS Block For Codec */
    wd_codec::reed_solomon::Block<code_length, fec_length> block;
        /* Instantiate RS Block For Codec */

    /* Instantiate Encoder and Decoder */
    typedef wd_codec::reed_solomon::Encoder<code_length, fec_length, data_length> Encoder;
    typedef wd_codec::reed_solomon::Decoder<code_length, fec_length, data_length> Decoder;

    typedef wd_codec::reed_solomon::File_Encoder<code_length, fec_length> file_encoder_t;
    typedef wd_codec::reed_solomon::File_Decoder<code_length, fec_length> file_decoder_t;

    void close() {
        wd_codec::Logger::close();
    }

    void setup() {

        Logger::init("logfile.txt");
        //Generate G(X)
        if (!wd_codec::create_root_generator_polynomial(field,
            generator_polynomial_index,
            generator_polynomial_root_count,
            generator_polynomial)) {
            //TODO: handle error.
            std::cout << "not good";
        }
        wd_codec::Logger::log(wd_codec::INFO, " G(x)= ", generator_polynomial);

    }

    

   






}
