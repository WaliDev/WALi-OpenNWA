#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "opennwa/Nwa.hpp"
#include "opennwa/NwaParser.hpp"
#include "opennwa/query/language.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cerr << "Syntax: " << argv[0] << " nwafilename\n";
        exit(1);
    }

    // Open the file
    ifstream infile(argv[1]);
    if (!infile.good()) {
        cerr << "Error opening input file " << argv[1] << "\n";
        exit(2);
    }

    opennwa::NwaRefPtr nwa = opennwa::read_nwa(infile);
    if (opennwa::query::languageIsEmpty(*nwa)) {
        cout << "THE NWA IS EMPTY\n";
    }
    else {
        cout << "THE NWA IS not EMPTY\n";
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
