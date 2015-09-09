#ifndef WALI_XFA_RELATIONMAKER_HPP
#define WALI_XFA_RELATIONMAKER_HPP

#include "wali/SemElem.hpp"
#include <wali/domains/binrel/BinRel_Interface.hpp>
#include <string>

namespace wali
{
    namespace xfa
    {
        struct RelationMaker
        {
            virtual
	    sem_elem_t
            initialize_variable_to_val(std::string const & varname,
                                       int val) const = 0;

            virtual
            sem_elem_t
            multiply_variable_by_two(std::string const & varname) const = 0;

            virtual
            sem_elem_t
            increment_variable(std::string const & varname) const = 0;

            virtual
            sem_elem_t
            assume_equality(std::string const & lhs_name,
                            std::string const & rhs_name) const = 0;

            virtual
            sem_elem_t
            zero() const = 0;

            virtual
            sem_elem_t
            one() const = 0;
        };
    }
}


#endif /* WALI_XFA_RELATIONMAKER_HPP */
