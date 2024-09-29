#include <fstream>
#include <cstddef> // For std::size_t
#include "Field.h"
template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
void mix_columns(const std::string& input_file, const std::string& output_file, std::size_t num_blocks) {
    std::ifstream in_stream(input_file, std::ios::binary);
    std::ofstream out_stream(output_file, std::ios::binary);

    if (!in_stream || !out_stream) {
        throw std::runtime_error("Error opening files.");
    }

    std::vector<wd_codec::galois::field_symbol> column_data(num_blocks); // Buffer for one column of data

    // Process columns
    for (std::size_t col = 0; col < code_length; ++col) {
        // Read the column data for all blocks
        for (std::size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
            in_stream.read(reinterpret_cast<char*>(&column_data[block_idx]), sizeof(wd_codec::galois::field_symbol));
            if (!in_stream) {
                throw std::runtime_error("Error reading from input file.");
            }
        }

        // Write the column data to output file (after mixing logic, if any)
        for (std::size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
            out_stream.write(reinterpret_cast<const char*>(&column_data[block_idx]), sizeof(wd_codec::galois::field_symbol));
            if (!out_stream) {
                throw std::runtime_error("Error writing to output file.");
            }
        }
    }

    in_stream.close();
    out_stream.close();
}


template <std::size_t code_length, std::size_t fec_length, std::size_t data_length = code_length - fec_length>
void unmix_columns(const std::string& input_file, const std::string& output_file, std::size_t num_blocks) {
    std::ifstream in_stream(input_file, std::ios::binary);
    std::ofstream out_stream(output_file, std::ios::binary);

    if (!in_stream || !out_stream) {
        throw std::runtime_error("Error opening files.");
    }

    std::vector<wd_codec::galois::field_symbol> column_data(num_blocks); // Buffer for one column of data

    // Process columns
    for (std::size_t col = 0; col < code_length; ++col) {
        // Read the column data for all blocks
        for (std::size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
            in_stream.read(reinterpret_cast<char*>(&column_data[block_idx]), sizeof(wd_codec::galois::field_symbol));
            if (!in_stream) {
                throw std::runtime_error("Error reading from input file.");
            }
        }

        // Write the column data to output file (after unmixing logic, if any)
        for (std::size_t block_idx = 0; block_idx < num_blocks; ++block_idx) {
            out_stream.write(reinterpret_cast<const char*>(&column_data[block_idx]), sizeof(wd_codec::galois::field_symbol));
            if (!out_stream) {
                throw std::runtime_error("Error writing to output file.");
            }
        }
    }

    in_stream.close();
    out_stream.close();
}
