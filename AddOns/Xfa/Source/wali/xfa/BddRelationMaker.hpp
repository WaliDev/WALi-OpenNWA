#ifndef WALI_XFA_BDD_RELATION_MAKER_HPP
#define WALI_XFA_BDD_RELATION_MAKER_HPP

#include "RelationMaker.hpp"

#include <wali/domains/binrel/ProgramBddContext.hpp>
#include <wali/domains/binrel/BinRel.hpp>

namespace wali
{
    namespace xfa
    {
        struct BddRelationMaker
            : RelationMaker
        {           
            wali::domains::binrel::ProgramBddContext const & voc;
            bdd ident;

            BddRelationMaker(wali::domains::binrel::ProgramBddContext const & v, bdd i)
                : voc(v)
                , ident(i)
            {}
        
            sem_elem_t
            initialize_variable_to_val(std::string const & varname,
                                       int val) const CPP11_OVERRIDE
            {
                using wali::domains::binrel::BinRel;                
                bdd b = voc.Assign(varname, voc.Const(val));
                sem_elem_t init = new BinRel(&voc, b);
                std::cerr << "> read: =0 bdd node count " << bdd_nodecount(b) << "\n";
                return init;
            }

            sem_elem_t
            multiply_variable_by_two(std::string const & varname) const CPP11_OVERRIDE
            {
                using wali::domains::binrel::BinRel;
                bdd b = voc.Assign(varname,
                                   voc.Plus(voc.From(varname), voc.From(varname)));
                sem_elem_t times2 = new BinRel(&voc, b);
                std::cerr << "> read: *2 bdd node count " << bdd_nodecount(b) << "\n";
                return times2;
            }

            sem_elem_t
            increment_variable(std::string const & varname) const CPP11_OVERRIDE
            {
                using wali::domains::binrel::BinRel;
                bdd b = voc.Assign(varname,
                                   voc.Plus(voc.From(varname), voc.Const(1)));
                sem_elem_t plus1 = new BinRel(&voc, b);
                std::cerr << "> read: +1 bdd node count " << bdd_nodecount(b) << "\n";
                return plus1;
            }    

            sem_elem_t
            assume_equality(std::string const & lhs_name,
                            std::string const & rhs_name) const CPP11_OVERRIDE
            {
                using namespace wali::domains::binrel;
                int lhs_fdd = voc.find(lhs_name)->second->baseLhs;
                int rhs_fdd = voc.find(rhs_name)->second->baseLhs;
                bdd eq = fdd_equals(lhs_fdd, rhs_fdd);
                binrel_t enforce_eq = new BinRel(&voc, eq & ident);
                return enforce_eq;
        
                bdd kill_counter = voc.Assign(rhs_name, voc.Const(0));
                binrel_t kill_k = new BinRel(&voc, kill_counter);

                return enforce_eq->Compose(kill_k.get_ptr());
            }

            sem_elem_t
            zero() const CPP11_OVERRIDE
            {
                using wali::domains::binrel::BinRel;
                return new BinRel(&voc, voc.False());        
            }

            sem_elem_t
            one() const CPP11_OVERRIDE
            {
                using wali::domains::binrel::BinRel;
                return new BinRel(&voc, ident);
            }
        };
    }
}

#endif /* WALI_XFA_BDD_RELATION_MAKER_HPP */
