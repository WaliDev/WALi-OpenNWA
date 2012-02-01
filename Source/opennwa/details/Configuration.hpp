#ifndef wali_nwa_CONFIGURATION_HPP
#define wali_nwa_CONFIGURATION_HPP

#include "opennwa/NwaFwd.hpp"

#include <ios>
#include <vector>

namespace opennwa
{
    namespace details
    {
        
        struct Configuration {
            State state;
            std::vector<State> callPredecessors;
            
            Configuration(State s) : state(s) {}
            Configuration(Configuration const & c)
                : state(c.state)
                , callPredecessors(c.callPredecessors) {}
            
            bool operator< (Configuration const & other) const {
                if (state < other.state) return true;
                if (state > other.state) return false;
                if (callPredecessors.size() < other.callPredecessors.size()) return true;
                if (callPredecessors.size() > other.callPredecessors.size()) return false;
                
                // Iterate in parallel over the two callPredecessors
                for (std::vector<State>::const_iterator i = callPredecessors.begin(), j = other.callPredecessors.begin();
                     i!=callPredecessors.end(); ++i, ++j)
                    {
                        assert (j!=other.callPredecessors.end());
                        if (*i < *j) return true;
                        if (*i > *j) return false;
                    }
          
                return false;
            }
            
            bool operator== (Configuration const & other) const {
                // If neither A < B nor B < A, then A == B
                return !(*this < other || other < *this);
            }
        };


        inline
        std::ostream &
        operator << (std::ostream & os, Configuration const & configuration)
        {
            os << wali::key2str(configuration.state) << "  [";
            for (size_t i=0; i<configuration.callPredecessors.size(); ++i) {
                os << wali::key2str(configuration.callPredecessors[i]) << " ";
            }
            os << "]";
            return os;
        }
        
    }
}

#endif
