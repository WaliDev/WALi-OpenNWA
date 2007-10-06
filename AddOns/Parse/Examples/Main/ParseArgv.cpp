#include "ParseArgv.hpp"
#include <cstdlib>

k::ParseArgv::ParseArgv( int a,char **b ) : argc(a),argv(b)
{
    parse();
}

k::ParseArgv::~ParseArgv() {}

bool
k::ParseArgv::get(
        const std::string &key,
        std::string &value ) const
{
    pamap_t::const_iterator it = m.find(key);
    if( it != m.end() ) {
        value = it->second;
        return true;
    }
    else
        return false;
}

bool
k::ParseArgv::geti( const std::string &key, int &i ) const
{
    std::string val;
    if( get(key,val) ) {
        // FIXME: insert error checking here
        i = atoi(val.c_str());
        return true;
    }
    else
        return false;
}

bool
k::ParseArgv::exists( const std::string &key ) const
{
    return (m.find(key) != m.end());
}

#if DBGPAV
#define PINS(i,t,v) std::cout << "idx : " << i << " Inserting ( " << t << " , " << v << " )\n";
#else
#define PINS(i,t,v)
#endif
//
//  Simply look for equal signs in the strings;
//
void
k::ParseArgv::parse()
{
    std::string::size_type idx = std::string::npos;
    std::string::size_type n;
    std::string tmp;
    for( int i=1; i < argc; i++ ) {
        tmp = argv[i];
        for( n = 0; n < tmp.size(); ) {
            idx = tmp.find('=',n);
            if( (idx == std::string::npos) ||
                ((idx > 0) && (tmp[idx-1] != '\\')) )
                break;
            else
                n = idx+1;
        }
        if( idx == std::string::npos ) {
            // must be a toggle flag,
            PINS(idx,tmp,"");
            m.insert( pamap_t::value_type(tmp,"") );
        }
        else {
            PINS(idx,tmp.substr(0,idx),tmp.substr(idx+1));
            m.insert( pamap_t::value_type(tmp.substr(0,idx),tmp.substr(idx+1)) );
        }
    }
}

