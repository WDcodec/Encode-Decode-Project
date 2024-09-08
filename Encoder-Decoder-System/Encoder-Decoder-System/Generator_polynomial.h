#include "Field.h"
#include "Field_Element.h"
#include "Polynomial.h"
#include "Logger.h"
//#include "Polynomial.cpp"
namespace wd_codec {
    inline bool create_root_generator_polynomial(const galois::Field& field,
        const std::size_t initial_index,
        const std::size_t num_elements,
        galois::Polynomial& generator_polynomial)
    {
        if (
            (initial_index >= field.size()) ||
            ((initial_index + num_elements) > field.size())
            )
        {
            wd_codec::Logger::log(wd_codec::CRITICAL, "Generator Polynomial - creation faild!");
            return false;
        }
        galois::Polynomial X = galois::generate_X(field);//polynomial X
        galois::Field_Element alpha(field, 2);//basic for all the root
        generator_polynomial = galois::Field_Element(field, 1);

        for (std::size_t i = initial_index; i < (initial_index + num_elements); i++) {
            //alpha^i is the root
            generator_polynomial *= (X + (alpha ^ static_cast<galois::field_symbol>(i)));
        }
        wd_codec::Logger::log(wd_codec::INFO, "Generator Polynomial - creation succeeded!, G(x)= ", generator_polynomial);
        return true;
    }

}