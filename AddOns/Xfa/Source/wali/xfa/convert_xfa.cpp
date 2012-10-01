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
        var_name(int id,
                 std::string const & domain_var_name_prefix)
        {
            return domain_var_name_prefix + "var" + boost::lexical_cast<std::string>(id);
        }    

        void
        register_vars(xfa_parser::Xfa const & ast,
                      wali::domains::binrel::ProgramBddContext & voc,
                      int fdd_size,
                      std::string const & prefix)
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
                    if (registered.find(var_name(act.operand_id, prefix)) == registered.end()) {
                        voc.addIntVar(var_name(act.operand_id, prefix), fdd_size);
                        std::cerr << "    " << var_name(act.operand_id, prefix) << "\n";
                        registered.insert(var_name(act.operand_id, prefix));
                    }
                }
            }
        }

        BinaryRelation
        get_relation(xfa_parser::Action const & act,
                     wali::domains::binrel::ProgramBddContext & voc,
                     BinaryRelation zero,
                     bdd ident,
                     std::string const & prefix)
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
                std::cerr << "      var " << var_name(act.operand_id, prefix) << "\n";
                std::cerr << "      to " << val << "\n";
                BinaryRelation ret = new BinRel(&voc, voc.Assign(var_name(act.operand_id, prefix),
                                                                 voc.Const(val)));
                std::cerr << "    done\n";
                return ret;
            }

            if (cmd.name == "incr") {
                std::cerr << "    incr\n";
                return new BinRel(&voc, voc.Assign(var_name(act.operand_id, prefix),
                                                   voc.Plus(voc.From(var_name(act.operand_id, prefix)),
                                                            voc.Const(1))));
            }

            if (cmd.name == "testnectr2") {
                std::cerr << "    testnectr2\n";
                assert(cmd.arguments.size() == 1u);
                assert(cmd.consequent);
                assert(!cmd.alternative);
                assert(cmd.consequent->action_type == "reject");

                int rhs_id = boost::lexical_cast<int>(cmd.arguments[0]);
                std::string lhs_name = var_name(act.operand_id, prefix);
                std::string rhs_name = var_name(rhs_id, prefix);
                
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
                     bdd ident,
                     std::string const & prefix)
        {
            std::cerr << "get_relation(trans):\n";
            using wali::domains::binrel::BinRel;
            if (trans.actions.size() == 0u) {
                return new BinRel(&voc, ident);
            }
            if (trans.actions.size() == 1u) {
                return get_relation(*trans.actions[0], voc, zero, ident, prefix);
            }
            assert(false);
        }


        Xfa
        from_parser_ast(xfa_parser::Xfa const & ast,
                        wali::domains::binrel::ProgramBddContext & voc,
                        int fdd_size,
                        std::string const & domain_var_name_prefix)
        {
            register_vars(ast, voc, fdd_size, domain_var_name_prefix);

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
                BinaryRelation rel = get_relation(**ast_trans, voc, zero, ident, domain_var_name_prefix);

                auto const & syms = (*ast_trans)->symbols;
                for (auto sym = syms.begin(); sym != syms.end(); ++sym) {
                    auto name = dynamic_cast<xfa_parser::Name*>(sym->get());
                    assert(name);
                    if (name->name == "epsilon") {
                        ret.addTrans(source, eps, dest, rel);
                    }
                    else {
                        ret.addTrans(source, getSymbol(name->name), dest, rel);
                    }
                }
            }

            ret.setInitialState(getState(ast.start_state));

            return ret;
        }
    }
}
