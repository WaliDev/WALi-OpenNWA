#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/NWAParser.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;

int main(int argc, char** argv)
{
    if (argc != 6 || argv[1] != std::string("-o") || argv[3] != std::string("-a")) {
        cerr << "Syntax: " << argv[0] << " -o dotfilename -a alphanwa nwafilename\n";
        exit(1);
    }

    // Open the files
    ifstream infile(argv[4]);
    if (!infile.good()) {
        cerr << "Error opening input file " << argv[3] << "\n";
        exit(2);
    }

    ifstream alphafile(argv[5]);
    if (!alphafile.good()) {
        cerr << "Error opening input file " << argv[3] << "\n";
        exit(2);
    }

    ofstream outfile(argv[2]);
    if (!outfile.good()) {
        cerr << "Error opening output file " << argv[2] << "\n";
        exit(3);
    }

    wali::nwa::NWARefPtr base_nwa = wali::nwa::read_nwa(infile);
    wali::nwa::NWARefPtr alpha_nwa = wali::nwa::read_nwa(alphafile);

    std::set<wali::nwa::Symbol> const & symbols = alpha_nwa->getSymbols();
    for(std::set<wali::nwa::Symbol>::const_iterator symb = symbols.begin();
        symb != symbols.end(); ++symb)
    {
        base_nwa->addSymbol(*symb);
    }
    
    base_nwa->print(outfile);
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
