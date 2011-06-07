#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>

#include "wali/nwa/NWA.hpp"
#include "wali/nwa/NWAParser.hpp"

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
    if (nwa->isEmpty()) {
        cout << "THE NWA IS EMPTY\n";
    }
    else {
        cout << "THE NWA IS not EMPTY\n";
    }
}


// Yo emacs!
// Local Variables:
//     c-basic-offset: 4
//     indent-tabs-mode: nil
// End:
