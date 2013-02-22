#ifndef WALI_XFA_BDD_RELATION_MAKER_HPP
#define WALI_XFA_BDD_RELATION_MAKER_HPP

#include "RelationMaker.hpp"

#include <wali/domains/binrel/ProgramBddContext.hpp>
#include <wali/domains/binrel/BinRel.hpp>

namespace details
{
    using namespace wali::domains::binrel;
    
    struct BddRelationMaker
        : RelationMaker
    {
        ProgramBddContext const & voc;
        bdd ident;

        BddRelationMaker(ProgramBddContext const & v, bdd i)
            : voc(v)
            , ident(i)
        {}
        
        BinaryRelation
        initialize_variable_to_val(std::string const & varname,
                                   int val) const CPP11_OVERRIDE
        {
            bdd b = voc.Assign(varname, voc.Const(val));
            BinaryRelation init = new BinRel(&voc, b);
            std::cerr << "> read: =0 bdd node count " << bdd_nodecount(b) << "\n";
            return init;
        }

        BinaryRelation
        multiply_variable_by_two(std::string const & varname) const CPP11_OVERRIDE
        {
            // x = x + x (where x is the __io_return being read into)
            bdd b = voc.Assign(varname,
                               voc.Plus(voc.From(varname), voc.From(varname)));
            BinaryRelation times2 = new BinRel(&voc, b);
            std::cerr << "> read: *2 bdd node count " << bdd_nodecount(b) << "\n";
            return times2;
        }

        BinaryRelation
        increment_variable(std::string const & varname) const CPP11_OVERRIDE
        {
            bdd b = voc.Assign(varname,
                               voc.Plus(voc.From(varname), voc.Const(1)));
            BinaryRelation plus1 = new BinRel(&voc, b);
            std::cerr << "> read: +1 bdd node count " << bdd_nodecount(b) << "\n";
            return plus1;
        }    

        BinaryRelation
        assume_equality(std::string const & lhs_name,
                        std::string const & rhs_name) const CPP11_OVERRIDE
        {
            int lhs_fdd = voc.find(lhs_name)->second->baseLhs;
            int rhs_fdd = voc.find(rhs_name)->second->baseLhs;
            bdd eq = fdd_equals(lhs_fdd, rhs_fdd);
            binrel_t enforce_eq = new BinRel(&voc, eq & ident);
            return enforce_eq;
        
            bdd kill_counter = voc.Assign(rhs_name, voc.Const(0));
            binrel_t kill_k = new BinRel(&voc, kill_counter);

            return enforce_eq->Compose(kill_k.get_ptr());                      
        }

        BinaryRelation
        zero() const CPP11_OVERRIDE
        {
            return new BinRel(&voc, voc.False());        
        }

        BinaryRelation
        one() const CPP11_OVERRIDE
        {
            return new BinRel(&voc, ident);
        }
    };
}

#endif /* WALI_XFA_BDD_RELATION_MAKER_HPP */
