#ifndef WALI_XFA_RELATIONMAKER_HPP
#define WALI_XFA_RELATIONMAKER_HPP

#include "Xfa.hpp"

#include <string>

namespace details
{
    using wali::xfa::BinaryRelation;
    
    struct RelationMaker
    {
        virtual
        BinaryRelation
        initialize_variable_to_val(std::string const & varname,
                                   int val) const = 0;

        virtual
        BinaryRelation
        multiply_variable_by_two(std::string const & varname) const = 0;

        virtual
        BinaryRelation
        increment_variable(std::string const & varname) const = 0;

        virtual
        BinaryRelation
        assume_equality(std::string const & lhs_name,
                        std::string const & rhs_name) const = 0;

        virtual
        BinaryRelation
        zero() const = 0;

        virtual
        BinaryRelation
        one() const = 0;
    };
}


#endif /* WALI_XFA_RELATIONMAKER_HPP */
