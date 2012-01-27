#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "opennwa/Nwa.hpp"
#include "opennwa/NestedWord.hpp"
#include "opennwa/NwaParser.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cerr << "Syntax: " << argv[0] << " nwafilename nested-word\n";
        exit(1);
    }

    // Open the file
    ifstream infile(argv[1]);
    if (!infile.good()) {
        cerr << "Error opening input file " << argv[1] << "\n";
        exit(2);
    }

    opennwa::NestedWord nw;

    std::stringstream ss(argv[2]);
    std::string s;
    while(ss >> s) {
        if (s[0] == '(') {
            s = s.substr(1);
            nw.appendCall(wali::getKey(s));
        }
        else if (s[0] == ')') {
            s = s.substr(1);
            nw.appendReturn(wali::getKey(s));
        }
        else {
            nw.appendInternal(wali::getKey(s));
        }
    }

    opennwa::NwaRefPtr nwa = opennwa::read_nwa(infile);
    if (nwa->isMemberNondet(nw)) {
        cout << "THE NWA CONTAINS THE GIVEN STRING\n";
    }
    else {
        cout << "THE NWA DOES not CONTAIN THE GIVEN STRING\n";
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
