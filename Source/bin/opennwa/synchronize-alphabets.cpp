#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include <boost/filesystem/convenience.hpp>

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

using namespace opennwa;


int main(int argc, char** argv)
{
    if (argc < 3) {
        cerr << "Syntax: " << argv[0] << " nwafilename nwafilename...\n";
        exit(1);
    }

    std::vector<std::string> filenames;
    for (int i=1; i<argc; ++i) {
        filenames.push_back(argv[i]);
    }

    // Read in all NWAs
    std::vector<NwaRefPtr> nwas;
    for (size_t i=0; i<filenames.size(); ++i) {
        ifstream infile(filenames[i].c_str());
        if (!infile.good()) {
            cerr << "Error opening input file " << argv[1] << "\n";
            exit(2);
        }
        nwas.push_back(opennwa::read_nwa(infile));
    }

    // Collect up all symbols
    std::set<opennwa::Symbol> symbols;
    for (size_t i=0; i<nwas.size(); ++i) {
        symbols.insert(nwas.at(i)->beginSymbols(), nwas.at(i)->endSymbols());
    }

    // Apply those symbols to each NWA
    for (size_t i=0; i<nwas.size(); ++i) {
        NwaRefPtr nwa = nwas[i];
        bool anything_new = false;
        for (std::set<opennwa::Symbol>::const_iterator symiter = symbols.begin();
             symiter != symbols.end(); ++symiter)
        {
            anything_new |= nwa->addSymbol(*symiter);
        }

        // If nothing was added, we won't rewrite the file. We'll indicate
        // this by setting that NWA ptr to NULL.
        if (!anything_new) {
            nwas[i] = NULL;
        }
    }

    // Write things back out
    assert(filenames.size() == nwas.size());
    for (size_t i=0; i<nwas.size(); ++i) {
        if (nwas.at(i) != NULL) {
            std::string bakname = filenames.at(i) + ".bak";
            if (boost::filesystem::exists(bakname)) {
                boost::filesystem::remove(bakname);
            }
            boost::filesystem::rename(filenames.at(i), bakname);

            ofstream outfile(filenames.at(i).c_str());

            nwas.at(i)->print(outfile);
        }
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
