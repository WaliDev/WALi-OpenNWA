#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/NWAParser.hpp"
#include "wali/nwa/query/language.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;
using std::vector;

extern vector<string> getWord(wali::nwa::NWA *aut);

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

    wali::nwa::NWARefPtr nwa = wali::nwa::read_nwa(infile);

    vector<string> word = getWord(nwa.get_ptr());

    for(size_t i=0; i<word.size(); ++i) {
        cout << i << ": " << word[i] << "\n";
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
