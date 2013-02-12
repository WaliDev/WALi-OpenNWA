#include "Xfa.hpp"
#include "ast.hpp"

#include <wali/domains/binrel/ProgramBddContext.hpp>
#include <wali/domains/binrel/BinRel.hpp>
#include <wali/util/DisjointSets.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <vector>
#include <sstream>

namespace details
{
extern
void
interleave_all_fdds();

extern
void
print_bdd_variable_order(std::ostream & os);
}


namespace wali {
    namespace xfa {

        struct ReadTransitionException {
            BinaryRelation back_weight, bit1_weight, init_weight;

            ReadTransitionException(BinaryRelation init, BinaryRelation back, BinaryRelation bit1)
                : back_weight(back)
                , bit1_weight(bit1)
                , init_weight(init)                  
            {}
        };

        struct WeightedTransition {
            State source;
            Symbol symbol;
            State target;
            BinaryRelation weight;
            
            WeightedTransition(State src, Symbol sym, State tgt, BinaryRelation w)
                : source(src), symbol(sym), target(tgt), weight(w)
            {
                assert (weight.get_ptr());
            }
        };

        typedef std::vector<WeightedTransition> TransList;
        

        std::string
        var_name(int id,
                 std::string const & domain_var_name_prefix)
        {
            std::stringstream os;
            os << domain_var_name_prefix
               << "var"
               << id;
            return os.str();
        }

        typedef wali::util::DisjointSets<std::string> StringSets;

        std::vector<std::map<std::string, int> >
        disjoint_sets_to_var_order(StringSets const & sets,
                                   int fdd_size)
        {
            std::vector<std::map<std::string, int> > answer;
            for (auto outer_iter = sets.begin(); outer_iter != sets.end(); ++outer_iter) {
                answer.push_back(std::map<std::string, int>());
                for (auto inner_iter = outer_iter->begin();
                     inner_iter != outer_iter->end(); ++inner_iter)
                {
                    answer.back()[*inner_iter] = fdd_size;
                }
            }
            return answer;
        }

        
        std::vector<std::map<std::string, int> >
        get_vars(XfaAst const & ast,
                      int fdd_size,
                      std::string const & prefix)
        {
            StringSets sets;
            
            std::cerr << "Registering variables with size " << fdd_size << "\n";
            std::set<std::string> registered;
            for (auto ast_trans = ast.transitions.begin(); ast_trans != ast.transitions.end(); ++ast_trans) {
                ast::ActionList & acts = (*ast_trans)->actions;
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
                        std::cerr << "    " << var_name(act.operand_id, prefix) << "\n";
                        registered.insert(var_name(act.operand_id, prefix));
                    }

                    auto cmd = *act.command;
                    if (cmd.name == "testnectr2") {
                        assert(cmd.arguments.size() == 1u);
                        assert(cmd.consequent);
                        assert(!cmd.alternative);
                        assert(cmd.consequent->action_type == "reject");

                        int rhs_id = boost::lexical_cast<int>(cmd.arguments[0]);
                        std::string lhs_name = var_name(act.operand_id, prefix);
                        std::string rhs_name = var_name(rhs_id, prefix);

                        sets.merge_sets(lhs_name, rhs_name);
                    }
                }
            }

            std::cerr << "Here is the set of variables: " << sets.to_string();


            std::vector<std::map<std::string, int> > vars = disjoint_sets_to_var_order(sets, fdd_size);
            vars.push_back(std::map<std::string, int>());

            //voc.addIntVar("left_current_state", ast.states.size());
            vars.back()[prefix + "current_state"] = static_cast<int>(ast.states.size()) * 2;
            std::cout << "Adding " << prefix << "current_state with size " << vars.back()[prefix + "current_state"];

            return vars;
            
            //voc.setIntVars(vars);
            //details::interleave_all_fdds();
            //::details::print_bdd_variable_order(std::cout);
        }

        BinaryRelation
        get_relation(ast::Action const & act,
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
                BinaryRelation times2, plus1, init;
                {
                    // x = 0
                    bdd b = voc.Assign(var_name(act.operand_id, prefix),
                                       voc.Const(0));
                    init = new BinRel(&voc, b);
                    std::cerr << "> read: =0 bdd node count " << bdd_nodecount(b) << "\n";
                }
                {
                    // x = x + x (where x is the __io_return being read into)
                    bdd b = voc.Assign(var_name(act.operand_id, prefix),
                                       voc.Plus(voc.From(var_name(act.operand_id, prefix)),
                                                voc.From(var_name(act.operand_id, prefix))));
                    times2 = new BinRel(&voc, b);
                    std::cerr << "> read: *2 bdd node count " << bdd_nodecount(b) << "\n";
                }
                {
                    // x = x + 1 (where x is the __io_return being read into)
                    bdd b = voc.Assign(var_name(act.operand_id, prefix),
                                       voc.Plus(voc.From(var_name(act.operand_id, prefix)),
                                                voc.Const(1)));
                    plus1 = new BinRel(&voc, b);
                    std::cerr << "> read: +1 bdd node count " << bdd_nodecount(b) << "\n";
                }
                throw ReadTransitionException(init, times2, plus1);
            }

            if (cmd.name == "reset") {
                assert(cmd.arguments.size() == 1u);
                int val = boost::lexical_cast<int>(cmd.arguments[0]);
                BinaryRelation ret = new BinRel(&voc, voc.Assign(var_name(act.operand_id, prefix),
                                                                 voc.Const(val)));
                return ret;
            }

            if (cmd.name == "incr") {
                return new BinRel(&voc, voc.Assign(var_name(act.operand_id, prefix),
                                                   voc.Plus(voc.From(var_name(act.operand_id, prefix)),
                                                            voc.Const(1))));
            }

            if (cmd.name == "testnectr2") {
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
                binrel_t enforce_eq = new BinRel(&voc, eq & ident);
                return enforce_eq;
                    
                bdd kill_counter = voc.Assign(rhs_name, voc.Const(0));
                binrel_t kill_k = new BinRel(&voc, kill_counter);

                return enforce_eq->Compose(kill_k.get_ptr());              
            }

            assert(false);
        }
        

        TransList
        get_transitions(ast::Transition const & trans,
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
                    auto name = dynamic_cast<ast::Name*>(sym->get());
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
                    auto name = dynamic_cast<ast::Name*>(sym->get());
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
                    assert (e.init_weight.get_ptr());
                    ret.push_back(WeightedTransition(source, startbits, intermediate, e.init_weight));

                    // intermediate --> dest has two transitions:
                    //     '__bit_0' has identity weight
                    //     '__bit_1' has +1 weight
                    ret.push_back(WeightedTransition(intermediate, bit0, dest, new BinRel(&voc, ident)));
                    ret.push_back(WeightedTransition(intermediate, bit1, dest, e.bit1_weight));

                    // dest --> intermediate has epsilon and weight *2
                    ret.push_back(WeightedTransition(dest, eps, intermediate, e.back_weight));
                }
            }

            return ret;
        }


        Xfa
        from_parser_ast(XfaAst const & ast,
                        wali::domains::binrel::ProgramBddContext & voc,
                        int fdd_size,
                        std::string const & domain_var_name_prefix)
        {
            using namespace wali::domains::binrel;
            BinaryRelation zero = new BinRel(&voc, voc.False());
            bdd ident = voc.Assume(voc.True(), voc.True());
            Symbol eps(wali::WALI_EPSILON);
            
            Xfa ret;

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
