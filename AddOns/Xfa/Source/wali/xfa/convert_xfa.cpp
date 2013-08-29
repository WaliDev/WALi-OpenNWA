#include "Xfa.hpp"
#include "ast.hpp"
#include "RelationMaker.hpp"

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
            sem_elem_t back_weight, bit1_weight, init_weight;

            ReadTransitionException(sem_elem_t init, sem_elem_t back, sem_elem_t bit1)
                : back_weight(back)
                , bit1_weight(bit1)
                , init_weight(init)                  
            {}
        };

        struct WeightedTransition {
            State source;
            Symbol symbol;
            State target;
            sem_elem_t weight;
            
            WeightedTransition(State src, Symbol sym, State tgt, sem_elem_t w)
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
            vars.back()[prefix + "current_state"] = static_cast<int>(ast.states.size()) * 4;
            std::cout << "Adding " << prefix << "current_state with size " << vars.back()[prefix + "current_state"];

            return vars;
            
            //voc.setIntVars(vars);
            //details::interleave_all_fdds();
            //::details::print_bdd_variable_order(std::cout);
        }

        sem_elem_t
        get_relation(RelationMaker const & maker,
                     ast::Action const & act,
                     std::string const & prefix)
        {
            using namespace wali::domains::binrel;

            if (act.action_type == "fire"
                || act.action_type == "alert")
            {
                std::cerr << "    fire or alert\n";
                // Hmmm.
                abort();
                return sem_elem_t();
            }

            assert(act.command);
            auto cmd = *act.command;

            if (cmd.name == "read") {
                sem_elem_t times2, plus1, init;
                std::string varname = var_name(act.operand_id, prefix);
                {
                    init = maker.initialize_variable_to_val(varname, 0);
                }
                {
                    times2 = maker.multiply_variable_by_two(varname);
                }
                {
                    plus1 = maker.increment_variable(varname);
                }
                throw ReadTransitionException(init, times2, plus1);
            }

            if (cmd.name == "reset") {
                assert(cmd.arguments.size() == 1u);
                int val = boost::lexical_cast<int>(cmd.arguments[0]);

                return maker.initialize_variable_to_val(var_name(act.operand_id, prefix),
                                                        val);
            }

            if (cmd.name == "incr") {
                return maker.increment_variable(var_name(act.operand_id, prefix));
            }

            if (cmd.name == "testnectr2") {
                assert(cmd.arguments.size() == 1u);
                assert(cmd.consequent);
                assert(!cmd.alternative);
                assert(cmd.consequent->action_type == "reject");

                int rhs_id = boost::lexical_cast<int>(cmd.arguments[0]);
                std::string lhs_name = var_name(act.operand_id, prefix);
                std::string rhs_name = var_name(rhs_id, prefix);
                
                return maker.assume_equality(lhs_name, rhs_name);
            }

            assert(false);
        }
        

        TransList
        get_transitions(RelationMaker const & maker,
                        ast::Transition const & trans,
                        std::string const & prefix)
        {
            State source = getState(trans.source);
            State dest = getState(trans.dest);
            Symbol eps(wali::WALI_EPSILON);            

            using wali::domains::binrel::BinRel;
            sem_elem_t rel;

            TransList ret;            

            try {
                if (trans.actions.size() == 0u) {
                    rel = maker.one();
                    //rel->is_effectively_one = true;
                }
                else if (trans.actions.size() == 1u) {
                    rel = get_relation(maker, *trans.actions[0], prefix);
                }
                else {
                    assert(false);
                }

                auto const & syms = trans.symbols;
                assert(syms.size() == 1u);
                for (auto sym = syms.begin(); sym != syms.end(); ++sym) {
                    auto name = dynamic_cast<ast::Name*>(sym->get());
                    assert(name);
                    if (name->name == "epsilon") {
                        ret.push_back(WeightedTransition(source, eps, dest, rel));
                    }
                    else {
                        throw ReadTransitionException(rel, maker.one(), maker.one());
                        //ret.push_back(WeightedTransition(source, getSymbol(name->name), dest, rel));
                    }
                }
            }
            catch (ReadTransitionException & e) {
                auto const & syms = trans.symbols;
                for (auto sym = syms.begin(); sym != syms.end(); ++sym) {
                    auto name = dynamic_cast<ast::Name*>(sym->get());
                    assert(name->name != "epsilon");

                    // source ---> intermediate_name ---> intermediate_2 --> dest
                    //                               <---
                    std::stringstream intermediate_name, intermediate2_name;
                    intermediate_name << trans.source << "__" << name->name << "_1";
                    intermediate2_name << trans.source << "__" << name->name << "_2";
                    std::stringstream bit0_name, bit1_name, start_name;
                    start_name << name->name << "__start";
                    bit0_name << name->name << "__0";
                    bit1_name << name->name << "__1";
                    Symbol startbits = getSymbol(start_name.str());
                    Symbol bit0 = getSymbol(bit0_name.str());
                    Symbol bit1 = getSymbol(bit1_name.str());
                    State intermediate = getState(intermediate_name.str());
                    State intermediate2 = getState(intermediate2_name.str());

                    // source --> intermediate has identity weight, symbol '__startbits'
                    assert (e.init_weight.get_ptr());
                    ret.push_back(WeightedTransition(source, startbits, intermediate, e.init_weight));

                    // intermediate --> intermediate2 has two transitions:
                    //     '__bit_0' has identity weight
                    //     '__bit_1' has +1 weight
                    ret.push_back(WeightedTransition(intermediate, bit0, intermediate2, maker.one()));
                    ret.push_back(WeightedTransition(intermediate, bit1, intermediate2, e.bit1_weight));

                    // intermediate2 --> intermediate has epsilon and weight *2
                    ret.push_back(WeightedTransition(intermediate2, eps, intermediate, e.back_weight));

                    // intermediate2 --> dest has epsilon and weight 1
                    ret.push_back(WeightedTransition(intermediate2, eps, dest, maker.one()));
                }
            }

            return ret;
        }


        Xfa
        from_parser_ast(RelationMaker const & maker,
                        XfaAst const & ast,
                        int UNUSED_PARAMETER(fdd_size),
                        std::string const & domain_var_name_prefix)
        {
            using namespace wali::domains::binrel;
            Symbol eps(wali::WALI_EPSILON);
            
            Xfa ret;

            for (auto ast_state = ast.states.begin(); ast_state != ast.states.end(); ++ast_state) {
                State state = getState((*ast_state)->name);
                ret.addState(state, maker.zero());
            }

            for (auto ast_trans = ast.transitions.begin(); ast_trans != ast.transitions.end(); ++ast_trans) {
                TransList transs = get_transitions(maker, **ast_trans, domain_var_name_prefix);
                for (auto trans = transs.begin(); trans != transs.end(); ++trans) {
                    ret.addTrans(trans->source, trans->symbol, trans->target, trans->weight);
                }
            }

            ret.setInitialState(getState(ast.start_state));

            return ret;
        }
    }
}


#include "BddRelationMaker.hpp"
