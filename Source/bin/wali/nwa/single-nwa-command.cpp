#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/NWAParser.hpp"

#include "wali/nwa/construct/constructions.hpp"
#include "wali/nwa/Expand.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;

using wali::nwa::NWARefPtr;
using wali::nwa::read_nwa;

using wali::nwa::CONSTRUCTION_NAMESPACE::CONSTRUCTION_FUNCTION;

int main(int argc, char** argv)
{
    if (argc != 4 || argv[1] != string("-o")) {
        cerr << "Syntax: " << argv[0] << " -o dotfilename nwafilename\n";
        exit(1);
    }

    // Open the files
    ifstream infile(argv[3]);
    if (!infile.good()) {
        cerr << "Error opening input file " << argv[3] << "\n";
        exit(2);
    }

    ofstream outfile(argv[2]);
    if (!outfile.good()) {
        cerr << "Error opening output file " << argv[2] << "\n";
        exit(3);
    }

    NWARefPtr nwa = read_nwa(infile);
    
    NWARefPtr out = CONSTRUCTION_FUNCTION(*nwa);
    
    out->print(outfile);
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
