#include "read_file.hpp"

#include <cassert>

namespace wali {
    namespace util {

        std::vector<char>
        read_file(FILE* file) {
            std::vector<char> data;
            const int block_size = 4096; // amt to read at once

            while (true) {
                assert(data.size() < 2000000000u);

                // Make room for another block
                data.resize(data.size() + block_size);
                assert(static_cast<long>(data.size()) - block_size >= 0);

                // Read into that new space
                int size_just_read = std::fread(&data[data.size()-block_size], 1u, block_size, file);
                assert(size_just_read <= block_size);

                if (size_just_read < block_size) {
                    // We hit EOF (or an error...) TODO better error handling
                    int excess = block_size - size_just_read;
                    data.resize(data.size() - excess);
                    return data;
                }
            }
        }

    }
}
