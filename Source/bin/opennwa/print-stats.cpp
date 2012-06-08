#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "opennwa/Nwa.hpp"
#include "opennwa/NwaParser.hpp"
#include "opennwa/query/automaton.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;

using namespace opennwa::query;

int main(int argc, char** argv)
{
    if (argc != 2 && argc != 3) {
        cerr << "Syntax: " << argv[0] << " [--json] nwafilename\n";
        exit(1);
    }

    if (argc == 3 && argv[1] != string("--json")) {
        cerr << "Syntax: " << argv[0] << " [--json] nwafilename\n";
        exit(1);
    }

    // Open the file
    ifstream infile(argv[argc-1]);
    
    if (!infile.good()) {
        cerr << "Error opening input file " << argv[argc-1] << "\n";
        exit(2);
    }

    opennwa::NwaRefPtr nwa = opennwa::read_nwa(infile);

    if (argc == 2) {
        // Print "human readable"
        std::cout << "Number of states:       " << nwa->sizeStates() << "\n";
        std::cout << "          initials:     " << nwa->sizeInitialStates() << "\n";
        std::cout << "          accepting:    " << nwa->sizeFinalStates() << "\n";
        std::cout << "          call sites:   " << numCallSites(*nwa) << "\n";
        std::cout << "          entry sites:  " << numEntrySites(*nwa) << "\n";
        std::cout << "          exit sites:   " << numExitSites(*nwa) << "\n";
        std::cout << "          return sites: " << numReturnSites(*nwa) << "\n";        
        std::cout << "\n";
        std::cout << "Number of symbols:      " << nwa->sizeSymbols() << "\n";
        std::cout << "\n";
        std::cout << "Number of transitions:  " << nwa->sizeTrans() << "\n";
        std::cout << "          internals:    " << nwa->sizeInternalTrans() << "\n";
        std::cout << "          calls:        " << nwa->sizeCallTrans() << "\n";
        std::cout << "          returns:      " << nwa->sizeReturnTrans() << "\n";
    }
    else {
        // Print JSON
        std::cout << "{\n";
        std::cout << "    \"count_states\":           " << nwa->sizeStates() << "\n";
        std::cout << "    \"count_initial_states\":   " << nwa->sizeInitialStates() << "\n";
        std::cout << "    \"count_accepting_states\": " << nwa->sizeFinalStates() << "\n";
        std::cout << "    \"count_call_sites\":       " << numCallSites(*nwa) << "\n";
        std::cout << "    \"count_entry_sites\":      " << numEntrySites(*nwa) << "\n";
        std::cout << "    \"count_exit_sites\":       " << numExitSites(*nwa) << "\n";
        std::cout << "    \"count_return_sites\":     " << numReturnSites(*nwa) << "\n";        
        
        std::cout << "    \"count_symbols\":          " << nwa->sizeSymbols() << "\n";
        
        std::cout << "    \"count_internals\":        " << nwa->sizeInternalTrans() << "\n";
        std::cout << "    \"count_calls\":            " << nwa->sizeCallTrans() << "\n";
        std::cout << "    \"count_returns\":          " << nwa->sizeReturnTrans() << "\n";
        std::cout << "}\n";
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
