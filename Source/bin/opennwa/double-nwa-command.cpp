#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "opennwa/Nwa.hpp"
#include "opennwa/NwaParser.hpp"

#include "opennwa/construct/constructions.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;

using opennwa::NwaRefPtr;
using opennwa::read_nwa;
using opennwa::construct::CONSTRUCTION_FUNCTION;

int main(int argc, char** argv)
{
    if (argc != 5 || argv[1] != std::string("-o")) {
        cerr << "Syntax: " << argv[0] << " -o dotfilename nwa1 nwa2\n";
        exit(1);
    }

    // Open the files
    ifstream infile1(argv[3]);
    if (!infile1.good()) {
        cerr << "Error opening input file " << argv[3] << "\n";
        exit(2);
    }

    ifstream infile2(argv[4]);
    if (!infile2.good()) {
        cerr << "Error opening input file " << argv[4] << "\n";
        exit(2);
    }

    ofstream outfile(argv[2]);
    if (!outfile.good()) {
        cerr << "Error opening output file " << argv[2] << "\n";
        exit(3);
    }

    NwaRefPtr nwa1 = read_nwa(infile1);
    NwaRefPtr nwa2 = read_nwa(infile2);
    
    NwaRefPtr result = CONSTRUCTION_FUNCTION(*nwa1, *nwa2);

    if (argv[2] != string("/dev/null") && argv[2] != string("NUL")) {
        result->print(outfile);
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
