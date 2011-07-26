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

using wali::nwa::NWARefPtr;
using wali::nwa::read_nwa;

int main(int argc, char** argv)
{
    if (argc != 3) {
        cerr << "Syntax: " << argv[0] << " nwa1 nwa2\n";
        exit(1);
    }

    // Open the files
    ifstream infile1(argv[1]);
    if (!infile1.good()) {
        cerr << "Error opening input file " << argv[1] << "\n";
        exit(2);
    }

    ifstream infile2(argv[2]);
    if (!infile2.good()) {
        cerr << "Error opening input file " << argv[2] << "\n";
        exit(2);
    }

    NWARefPtr nwa1 = read_nwa(infile1);
    NWARefPtr nwa2 = read_nwa(infile2);

    bool result = wali::nwa::query::languageSubsetEq(*nwa1, *nwa2);

    if (result) {
        cout << "The language of the first is a subset of the second\n";
    }
    else {
        cout << "The language of the first is NOT a subset of the second\n";
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
