//for fileio- file_size function
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>
namespace wd_codec {

    namespace reed_solomon {
        class Fileio
        {
            inline std::size_t file_size(const std::string& file_name)
            {
                std::ifstream file(file_name.c_str(), std::ios::binary);
                if (!file) return 0;
                file.seekg(0, std::ios::end);
                return static_cast<std::size_t>(file.tellg());
            }

        };
    }
}
