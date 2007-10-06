#ifndef PARSE_ARG_V_GUARD
#define PARSE_ARG_V_GUARD
#include <iostream>
#include <map>

namespace k
{

    //
    // Takes command line arguments and splits each one on a non escaped
    // equal sign ('=').  ie. key=value
    // Inserts the pair (key,val) into a map.  If no equal sign is present
    // inserts (key,"") into the map.
    //
    // - get* methods do not side effect their second parameter if the key
    //      does not exist
    //
    class ParseArgv
    {

        public:
            ParseArgv( int argc,char **argv );
            virtual ~ParseArgv();

            virtual bool get(
                    const std::string &key,
                    std::string &value ) const;

            virtual bool geti( const std::string &key, int &i ) const;

            virtual bool exists( const std::string &key ) const;


        protected:
            typedef std::map< std::string, std::string > pamap_t;
            virtual void parse();

        protected:
            int argc;
            char ** argv;
            pamap_t m;

    }; // class ParseArgv

} // namespace k

#endif  // PARSE_ARG_V_GUARD
