#include "Xfa.hpp"
#include "xfa-parser/ast.hh"
#include "wali/domains/binrel/ProgramBddContext.hpp"
#include "wali/domains/binrel/BinRel.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <vector>

using namespace xfa_parser;

namespace cfglib {
    namespace xfa {

        struct ReadTransitionException {
            BinaryRelation back_weight, bit1_weight;

            ReadTransitionException(BinaryRelation back, BinaryRelation bit1)
                : back_weight(back)
                , bit1_weight(bit1)
            {}
        };

        struct WeightedTransition {
            State source;
            Symbol symbol;
            State target;
            BinaryRelation weight;
            
            WeightedTransition(State src, Symbol sym, State tgt, BinaryRelation w)
                : source(src), symbol(sym), target(tgt), weight(w)
            {}
        };

        typedef std::vector<WeightedTransition> TransList;
        

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
            static bool has_run = false;
            assert(!has_run);
            
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
                        if (has_run) {
                            voc.addIntVar(var_name(act.operand_id, prefix), fdd_size);
                        }
                        std::cerr << "    " << var_name(act.operand_id, prefix) << "\n";
                        registered.insert(var_name(act.operand_id, prefix));
                    }
                }
            }


            if (!has_run) {
                std::map<std::string, int> vars;
                for (auto var = registered.begin(); var != registered.end(); ++var) {
                    //voc.addIntVar(*var, fdd_size);
                    vars[*var] = fdd_size;
                }

                //voc.addIntVar("left_current_state", ast.states.size());
                vars["left_current_state"] = ast.states.size();
                
                voc.setIntVars(vars);
                has_run = true;
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
                // x = x + x (where x is the __io_return being read into)
                BinaryRelation times2 = new BinRel(&voc, voc.Assign(var_name(act.operand_id, prefix),
                                                                    voc.Plus(voc.From(var_name(act.operand_id, prefix)),
                                                                             voc.From(var_name(act.operand_id, prefix)))));
                // x = x + 1 (where x is the __io_return being read into)
                BinaryRelation plus1 = new BinRel(&voc, voc.Assign(var_name(act.operand_id, prefix),
                                                                   voc.Plus(voc.From(var_name(act.operand_id, prefix)),
                                                                            voc.Const(1))));
                throw ReadTransitionException(times2, plus1);
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
        

        TransList
        get_transitions(xfa_parser::Transition const & trans,
                        wali::domains::binrel::ProgramBddContext & voc,
                        BinaryRelation zero,
                        bdd ident,
                        std::string const & prefix)
        {
            State source = getState(trans.source);
            State dest = getState(trans.dest);
            Symbol eps(wali::WALI_EPSILON);            

            using wali::domains::binrel::BinRel;
            BinaryRelation rel;

            TransList ret;            

            try {
                if (trans.actions.size() == 0u) {
                    rel = new BinRel(&voc, ident);
                    //rel->is_effectively_one = true;
                }
                else if (trans.actions.size() == 1u) {
                    rel = get_relation(*trans.actions[0], voc, zero, ident, prefix);
                }
                else {
                    assert(false);
                }

                auto const & syms = trans.symbols;
                for (auto sym = syms.begin(); sym != syms.end(); ++sym) {
                    auto name = dynamic_cast<xfa_parser::Name*>(sym->get());
                    assert(name);
                    if (name->name == "epsilon") {
                        ret.push_back(WeightedTransition(source, eps, dest, rel));
                    }
                    else {
                        ret.push_back(WeightedTransition(source, getSymbol(name->name), dest, rel));
                    }
                }
            }
            catch (ReadTransitionException & e) {
                auto const & syms = trans.symbols;
                for (auto sym = syms.begin(); sym != syms.end(); ++sym) {
                    auto name = dynamic_cast<xfa_parser::Name*>(sym->get());
                    assert(name->name != "epsilon");

                    // source ---> intermediate_name ---> dest
                    //                               <---
                    std::stringstream intermediate_name;
                    intermediate_name << trans.source << "__" << name->name;
                    std::stringstream bit0_name, bit1_name;;
                    bit0_name << name->name << "__bit_is_0";
                    bit1_name << name->name << "__bit_is_1";
                    Symbol startbits = getSymbol("__startbits");
                    Symbol bit0 = getSymbol(bit0_name.str());
                    Symbol bit1 = getSymbol(bit1_name.str());
                    State intermediate = getState(intermediate_name.str());

                    // source --> intermediate has identity weight, symbol '__startbits'
                    rel = new BinRel(&voc, ident);
                    ret.push_back(WeightedTransition(source, startbits, intermediate, rel));

                    // intermediate --> dest has two transitions:
                    //     '__bit_0' has identity weight
                    //     '__bit_1' has +1 weight
                    ret.push_back(WeightedTransition(intermediate, bit0, dest, rel));
                    ret.push_back(WeightedTransition(intermediate, bit1, dest, e.bit1_weight));

                    // dest --> intermediate has epsilon and weight *2
                    ret.push_back(WeightedTransition(dest, eps, intermediate, e.back_weight));
                }
            }

            return ret;
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
                TransList transs = get_transitions(**ast_trans, voc, zero, ident, domain_var_name_prefix);
                for (auto trans = transs.begin(); trans != transs.end(); ++trans) {
                    ret.addTrans(trans->source, trans->symbol, trans->target, trans->weight);
                }
            }

            ret.setInitialState(getState(ast.start_state));

            return ret;
        }
    }
}
