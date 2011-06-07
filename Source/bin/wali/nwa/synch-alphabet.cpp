#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "wali/nwa/NWA.hpp"

//#include <boost/program_options/options_description.hpp>
//#include <boost/program_options/positional_options.hpp>
//#include <boost/program_options/variables_map.hpp>
//#include <boost/program_options/parsers.hpp>

//namespace po = boost::program_options;

using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::exit;

int main(int argc, char** argv)
{
    //po::options_description options("Allowed options");

    //options.add_options()
    //    ("help", "Output help message")
    //    ("input-file", po::value<vector<string>>(), "The file to input")
    //     ("output-file", po::value<vector<string>>(), "The file to write")
    //   ;

    //po::variables_map args;
    //po::store(po::parse_command_line(argc, argv, options), args);
    //po::notify(args);

    //if (args.count("help")) {
    //    cerr << options << endl;
    //    return 1;
    //}


    //if (args.count("input-file")) {
    //    cout << "Read from " << args["input-file"].as<vector<string>>() << endl;
    //}
    //if (args.count("output-file")) {
    //    cout << "Write to " << args["output-file"].as<vector<string>>() << endl;
    //}

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

    std::set<wali::nwa::Sym> const & symbols = alpha_nwa->getSymbols();
    for(std::set<wali::nwa::Sym>::const_iterator symb = symbols.begin();
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
