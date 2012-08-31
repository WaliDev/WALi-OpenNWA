#include "Xfa.hpp"
#include "xfa-parser/ast.hh"
#include "wali/domains/binrel/ProgramBddContext.hpp"
#include "wali/domains/binrel/BinRel.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>

using namespace xfa_parser;

namespace cfglib {
    namespace xfa {

        std::string
        var_name(int id)
        {
            return "var" + boost::lexical_cast<std::string>(id);
        }    

        void
        register_vars(xfa_parser::Xfa const & ast,
                      wali::domains::binrel::ProgramBddContext & voc,
                      int fdd_size)
        {
            std::cerr << "Registering variables with size " << fdd_size << "\n";
            std::set<std::string> registered;
            for (auto ast_trans = ast.transitions.begin(); ast_trans != ast.transitions.end(); ++ast_trans) {
                ActionList & acts = (*ast_trans)->actions;
                for (auto act_it = acts.begin(); act_it != acts.end(); ++act_it) {
                    auto & act = **act_it;
                    if (act.action_type == "fire"
                        || act.action_type == "alert"
                        || act.action_type == "reject")
                    {
                        continue;
                    }
                    assert (act.action_type == "ctr2");
                    if (registered.find(var_name(act.operand_id)) == registered.end()) {
                        voc.addIntVar(var_name(act.operand_id), fdd_size);
                        std::cerr << "    " << var_name(act.operand_id) << "\n";
                        registered.insert(var_name(act.operand_id));
                    }
                }
            }
        }

        BinaryRelation
        get_relation(xfa_parser::Action const & act,
                     wali::domains::binrel::ProgramBddContext & voc,
                     BinaryRelation zero,
                     bdd ident)
        {
            using namespace wali::domains::binrel;

            if (act.action_type == "fire"
                || act.action_type == "alert")
            {
                std::cerr << "    fire or alert\n";
                // Hmmm.
                return zero;
            }

            assert(act.command);
            auto cmd = *act.command;

            if (cmd.name == "read") {
                // FIXME
                std::cerr << "    read\n";
                return zero;
            }

            if (cmd.name == "reset") {
                std::cerr << "    reset\n";
                assert(cmd.arguments.size() == 1u);
                int val = boost::lexical_cast<int>(cmd.arguments[0]);
                std::cerr << "      var " << var_name(act.operand_id) << "\n";
                std::cerr << "      to " << val << "\n";
                BinaryRelation ret = new BinRel(&voc, voc.Assign(var_name(act.operand_id),
                                                                 voc.Const(val)));
                std::cerr << "    done\n";
                return ret;
            }

            if (cmd.name == "incr") {
                std::cerr << "    incr\n";
                return new BinRel(&voc, voc.Assign(var_name(act.operand_id),
                                                   voc.Plus(voc.From(var_name(act.operand_id)),
                                                            voc.Const(1))));
            }

            if (cmd.name == "testnectr2") {
                std::cerr << "    testnectr2\n";
                assert(cmd.arguments.size() == 1u);
                assert(cmd.consequent);
                assert(!cmd.alternative);
                assert(cmd.consequent->action_type == "reject");

                int rhs_id = boost::lexical_cast<int>(cmd.arguments[0]);
                std::string lhs_name = var_name(act.operand_id);
                std::string rhs_name = var_name(rhs_id);
                
                int lhs_fdd = voc[lhs_name]->baseLhs;
                int rhs_fdd = voc[rhs_name]->baseLhs;
                bdd eq = fdd_equals(lhs_fdd, rhs_fdd);
                return new BinRel(&voc, eq & ident);
            }

            assert(false);
        }
        

        BinaryRelation
        get_relation(xfa_parser::Transition const & trans,
                     wali::domains::binrel::ProgramBddContext & voc,
                     BinaryRelation zero,
                     bdd ident)
        {
            std::cerr << "get_relation(trans):\n";
            using wali::domains::binrel::BinRel;
            if (trans.actions.size() == 0u) {
                return new BinRel(&voc, ident);
            }
            if (trans.actions.size() == 1u) {
                return get_relation(*trans.actions[0], voc, zero, ident);
            }
            assert(false);
        }


        Xfa
        from_parser_ast(xfa_parser::Xfa const & ast,
                        wali::domains::binrel::ProgramBddContext & voc,
                        int fdd_size)
        {
            register_vars(ast, voc, fdd_size);

            using namespace wali::domains::binrel;
            BinaryRelation zero = new BinRel(&voc, voc.False());
            bdd ident = voc.Assume(voc.True(), voc.True());
            Symbol eps(wali::WALI_EPSILON);
            
            cfglib::xfa::Xfa ret;

            for (auto ast_state = ast.states.begin(); ast_state != ast.states.end(); ++ast_state) {
                State state = getState((*ast_state)->name);
                ret.addState(state, zero);
            }

            for (auto ast_trans = ast.transitions.begin(); ast_trans != ast.transitions.end(); ++ast_trans) {
                State source = getState((*ast_trans)->source);
                State dest = getState((*ast_trans)->dest);
                BinaryRelation rel = get_relation(**ast_trans, voc, zero, ident);
                ret.addTrans(source, eps, dest, rel);
            }

            ret.setInitialState(getState(ast.start_state));

            return ret;
        }
    }
}
