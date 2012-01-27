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
using std::vector;

using opennwa::NestedWord;
using opennwa::NestedWordRefPtr;


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

    NestedWordRefPtr word = opennwa::query::getSomeAcceptedWord(*nwa);

    for(NestedWord::const_iterator it = word->begin(); it!=word->end(); ++it) {
        cout << it->type << ": [" << it->symbol << "] " << wali::key2str(it->symbol) << "\n";
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
