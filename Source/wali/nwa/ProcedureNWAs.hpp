#ifndef PROCEDURE_NWAS_HPP
#define PROCEDURE_NWAS_HPP

#include "NWA.hpp"

#include <map>
#include <string>

namespace wali {
    namespace nwa {
        // Maps name of procedure to the procedure NWA
        typedef std::map<std::string, NWARefPtr> ProcedureMap;


        NWARefPtr
        assemble_nwa(ProcedureMap const & procedures, Key call_key, Key return_key);

    }
}
        
#endif
