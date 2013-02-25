#include "wali/xfa/Xfa.hpp"
#include "wali/xfa/ast.hpp"
#include "wali/xfa/RelationMaker.hpp"
#include "wali/wpds/WPDS.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <vector>

#include "wali/domains/genkill/GenKillXformerTemplate.hpp"

using namespace wali::xfa;
using namespace wali;
using namespace wali::wfa;
using namespace wali::wpds;

struct VarSet : public std::set<std::string>
{
    static VarSet & UniverseSet()
    {
        static VarSet u;
        return u;
    }
    
    static VarSet EmptySet()
    {
        return VarSet();
    }
    
    static bool Eq( const VarSet& x, const VarSet& y ) {
        return x == y;
    }

    static VarSet Diff( const VarSet& x, const VarSet& y,
                        bool UNUSED_PARAMETER(normalizing) = false )
    {      
        VarSet ret;
        std::set_difference(x.begin(), x.end(),
                            y.begin(), y.end(),
                            std::inserter(ret, ret.begin()));
        return ret;
    }
    
    static VarSet Union( const VarSet& x, const VarSet& y )
    {
        VarSet ret;
        std::set_union(x.begin(), x.end(),
                       y.begin(), y.end(),
                       std::inserter(ret, ret.begin()));
        return ret;
    }        

    static VarSet Intersect( const VarSet& x, const VarSet& y )
    {
        VarSet ret;
        std::set_intersection(x.begin(), x.end(),
                              y.begin(), y.end(),
                              std::inserter(ret, ret.begin()));
        return ret;
    }        

    std::ostream& print(std::ostream& o) const
    {
        o << "{";
        for (VarSet::const_iterator iter = this->begin();
             iter != this->end(); ++iter)
        {
            o << *iter << ", ";
        }
        o << "}";
        return o;
    }

};

typedef GenKillTransformer_T<VarSet> GenKillWeight;

sem_elem_t
makeGKW(VarSet const & kill, VarSet const & gen)
{
    return GenKillWeight::makeGenKillTransformer_T(kill, gen);
}        



namespace wali {
    namespace xfa {

        namespace details {
        
        std::string
        var_name(int id,
                 std::string const & domain_var_name_prefix)
        {
            return domain_var_name_prefix + "var" + boost::lexical_cast<std::string>(id);
        }    



        void
        register_vars(ast::XfaAst const & ast,
                      std::string const & prefix)
        {
            VarSet & all_vars = VarSet::UniverseSet();
            
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
                    all_vars.insert(var_name(act.operand_id, prefix));
                }
            }
            
        }
        

        struct ReadTransitionException {
            sem_elem_t back_weight, bit1_weight, init_weight;

            ReadTransitionException(sem_elem_t init, sem_elem_t back, sem_elem_t bit1)
                : back_weight(back)
                , bit1_weight(bit1)
                , init_weight(init)
            {}
        };

        struct WeightedTransition {
            Key source;
            Key symbol;
            Key target;
            sem_elem_t weight;
            
            WeightedTransition(Key src,
                               Key sym,
                               Key tgt,
                               sem_elem_t w)
                : source(src), symbol(sym), target(tgt), weight(w)
            {}
        };

        typedef std::vector<WeightedTransition> TransList;
        

        sem_elem_t
        get_relation(ast::Action const & act,
                     std::string const & prefix)
        {
            VarSet empty;
            sem_elem_t zero = makeGKW(empty, empty);
            
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
                VarSet killed;
                killed.insert(var_name(act.operand_id, prefix));

                sem_elem_t kill_weight = makeGKW(killed, empty);
                
                throw ReadTransitionException(kill_weight, zero, zero);
            }

            if (cmd.name == "reset") {
                VarSet killed;
                assert(cmd.arguments.size() == 1u);
                killed.insert(var_name(act.operand_id, prefix));
                sem_elem_t kill_weight = makeGKW(killed, empty);
                return kill_weight;
            }

            if (cmd.name == "incr") {
                VarSet saved;
                saved.insert(var_name(act.operand_id, prefix));
                sem_elem_t save_weight = makeGKW(empty, saved);
                return save_weight;
            }

            if (cmd.name == "testnectr2") {
                assert(cmd.arguments.size() == 1u);
                assert(cmd.consequent);
                assert(!cmd.alternative);
                assert(cmd.consequent->action_type == "reject");

                int rhs_id = boost::lexical_cast<int>(cmd.arguments[0]);
                std::string lhs_name = var_name(act.operand_id, prefix);
                std::string rhs_name = var_name(rhs_id, prefix);

                VarSet saved;
                saved.insert(lhs_name);
                saved.insert(rhs_name);
                sem_elem_t save_weight = makeGKW(empty, saved);

                return save_weight;
            }

            assert(false);
        }
        

        TransList
        get_transitions(ast::Transition const & trans,
                        std::string const & prefix)
        {
            Key source = getKey(trans.source);
            Key dest = getKey(trans.dest);
            Key eps(wali::WALI_EPSILON);            

            TransList ret;
            VarSet empty;
            sem_elem_t rel = makeGKW(empty, empty);

            try {
                if (trans.actions.size() == 0u) {
                    //rel->is_effectively_one = true;
                }
                else if (trans.actions.size() == 1u) {
                    rel = get_relation(*trans.actions[0], prefix);
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
                        ret.push_back(WeightedTransition(source, getKey(name->name), dest, rel));
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
                    Key startbits = getKey("__startbits");
                    Key bit0 = getKey(bit0_name.str());
                    Key bit1 = getKey(bit1_name.str());
                    Key intermediate = getKey(intermediate_name.str());

                    // source --> intermediate has identity weight, symbol '__startbits'
                    ret.push_back(WeightedTransition(source, startbits, intermediate, e.init_weight));

                    // intermediate --> dest has two transitions:
                    //     '__bit_0' has identity weight
                    //     '__bit_1' has +1 weight
                                  ret.push_back(WeightedTransition(intermediate, bit0, dest, e.init_weight->one()));
                    ret.push_back(WeightedTransition(intermediate, bit1, dest, e.bit1_weight));

                    // dest --> intermediate has epsilon and weight *2
                    ret.push_back(WeightedTransition(dest, eps, intermediate, e.back_weight));
                }
            }

            return ret;
        }


        wali::wfa::WFA
        create_live_var_wfa(ast::XfaAst const & ast,
                            std::string const & domain_var_name_prefix)
        {
            register_vars(ast, domain_var_name_prefix);
            
            wali::wfa::WFA ret;

            VarSet empty;
            sem_elem_t zero = makeGKW(empty, empty);
            zero = zero->zero();

            for (auto ast_state = ast.states.begin(); ast_state != ast.states.end(); ++ast_state) {
                Key state = getKey((*ast_state)->name);
                ret.addState(state, zero);
            }

            for (auto ast_trans = ast.transitions.begin(); ast_trans != ast.transitions.end(); ++ast_trans) {
                TransList transs = get_transitions(**ast_trans, domain_var_name_prefix);
                for (auto trans = transs.begin(); trans != transs.end(); ++trans) {
                    ret.addTrans(trans->source, trans->symbol, trans->target, trans->weight);
                }
            }

            ret.setInitialState(getKey(ast.start_state));

            return ret;
        }


        struct VarKiller : TransFunctor
        {
            WFA * gen_kill_results;
            Key p_state;
            RelationMaker * maker;

            VarSet get_live(Key state) {
                TransSet matches = gen_kill_results->match(p_state, state);
                if (matches.size() == 0) {
                    std::cerr << "WARNING: no prestar result for " << key2str(state) << "\n";
                    return VarSet();
                }
                if (matches.size() > 1) {
                    assert(false);
                }
                sem_elem_t w = (*(matches.begin()))->weight();
                GenKillWeight * gkw = dynamic_cast<GenKillWeight *>(w.get_ptr());
                //assert(gkw->getKill().empty());
                return gkw->getGen();
            }

            void
            operator() (ITrans * trans) {
                VarSet
                    source_live = get_live(trans->from()),
                    target_live = get_live(trans->to());

                VarSet killed = VarSet::Diff(source_live, target_live);

                for (VarSet::const_iterator v = killed.begin(); v != killed.end(); ++v) {
                    std::cout << "==== Killing variable " << *v << " on transition "
                              << key2str(trans->from()) << "--" << key2str(trans->stack()) << "--> "
                              << key2str(trans->to()) << "\n";

                    sem_elem_t kill_relation = maker->initialize_variable_to_val(*v, 0);
                    sem_elem_t new_weight = trans->weight()->extend(kill_relation);

                    trans->setWeight(new_weight);
                }
            }
        };

            
        bool
        always_include_trans(ITrans const * UNUSED_PARAMETER(t))
        {
            return true;
        }
        

        void
        kill_variables(ast::XfaAst const & ast,
                       wali::xfa::Xfa & xfa,
                       RelationMaker & maker,
                       std::string const & domain_var_name_prefix)
        {
            WFA wfa_genkill = create_live_var_wfa(ast, domain_var_name_prefix);

            // do prestar from the final state
            WFA query;
            Key p_state = getKey("__p");
            Key accept = getKey("__accept");
            Key target_accept = getKey("accept");

            query.addTrans(p_state, target_accept, accept,
                           wfa_genkill.getSomeWeight()->one());

            WPDS wpds;
            wfa_genkill.toWpds(p_state, &wpds, always_include_trans);
            WFA result = wpds.prestar(query);

            //
            VarKiller killer;
            killer.gen_kill_results = &result;
            killer.p_state = p_state;
            killer.maker = &maker;

            xfa.for_each(killer);
        }

        }
        
    }
}
