#include "Xfa.hpp"
#include "xfa-parser/ast.hh"
#include "wali/domains/binrel/ProgramBddContext.hpp"
#include "wali/domains/binrel/BinRel.hpp"


using namespace xfa_parser;

namespace cfglib {
    namespace xfa {

        Xfa
        from_parser_ast(xfa_parser::Xfa const & ast,
                        wali::domains::binrel::ProgramBddContext & voc)
        {
            using namespace wali::domains::binrel;
            BinaryRelation zero = new BinRel(&voc, voc.False());
            Symbol eps(wali::WALI_EPSILON);
            
            cfglib::xfa::Xfa ret;

            for (auto ast_state : ast.states) {
                State state = getState(ast_state->name);
                ret.addState(state, zero);
            }

            for (auto ast_trans : ast.transitions) {
                State source = getState(ast_trans->source);
                State dest = getState(ast_trans->dest);
                ret.addTrans(source, eps, dest, zero);
            }

            ret.setInitialState(getState(ast.start_state));

            return ret;
        }
    }
}
