#include "NWAParser.hpp"

int
main(int argc, char** argv)
{
    (void)argv;
    
    test_all();

    if (argc > 1) {
        std::cerr << "\n>> Tests good!\n";
        std::cerr << "\n****\nThere should be an error here; this is making sure assert is enabled\n";
        assert(false);
    }
}
