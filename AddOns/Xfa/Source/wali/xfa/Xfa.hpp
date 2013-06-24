#ifndef INCLUDE_WALI_XFA_XFA_HPP
#define INCLUDE_WALI_XFA_XFA_HPP

#include <wali/Common.hpp>

#include <boost/bimap.hpp>

#include "ast.hpp"
#include "RelationMaker.hpp"

#include <wali/wfa/WFA.hpp>
#include <wali/wfa/WeightMaker.hpp>
#include <wali/wfa/TransFunctor.hpp>
#include <wali/wfa/ITrans.hpp>
#include <wali/domains/binrel/BinRel.hpp>
#include <wali/domains/binrel/ProgramBddContext.hpp>
#include <wali/Key.hpp>
#include <wali/util/read_file.hpp>
#include <wali/util/base64.hpp>
#include <wali/util/map_at.hpp>

#include <opennwa/Nwa.hpp>

#include <map>
#include <sstream>
#include <fstream>
#include <cstdio>

#define CPP11_OVERRIDE


namespace wali {
    namespace xfa {

        namespace details {
            class IntersectionWeightMaker
                : public wali::wfa::WeightMaker
            {
                virtual sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs ) CPP11_OVERRIDE;
            };
        }
        

        /// Represent an XFA state.
        ///
        /// This is a thin wrapper around wali::Key -- made separate because
        /// I like the extra type safety it provides.
        struct State {
            wali::Key key;
            
            explicit State(wali::Key k)
                : key(k)
            {}
        };

        
        inline
        bool operator< (State left, State right) {
            return left.key < right.key;
        }


        /// Represent a symbol on an XFA transition.
        ///
        /// This is a thin wrapper around wali::Key -- made separate because
        /// I like the extra type safety it provides.
        struct Symbol {
            wali::Key key;

            explicit Symbol(wali::Key k)
                : key(k)
            {}
        };


        /// Wrapper around wali::getKey that returns a State.
        ///
        /// Again, type safety.
        inline
        State getState(std::string const & str) {
            return State(wali::getKey(str));
        }


        /// Wrapper around wali::getKey that returns a Symbol.
        ///
        /// Again, type safety.
        inline
        Symbol getSymbol(std::string const & str) {
            return Symbol(wali::getKey(str));
        }

        
        /// An XFA is an "eXtended Finite Automaton".
        ///
        /// An XFA is a standard FA extended with an additional data domain
        /// D. This essentially serves as an additional component of the
        /// machine's state, which is then Q x D.
        ///
        /// The machine chooses a transition based on the current state and
        /// the input symbol; that transition is associated with a (possibly
        /// nondeterministic) state transformer which describes how the data
        /// state may change change. The machine picks a choice and
        /// continues.
        ///
        /// An XFA specification gives a set of starting configurations
        /// (pairs from Q x D) and a set of accepting configurations, but
        /// these are not well-modeled by this class at this point in
        /// time. Possibly these should move into the WFA class.
        ///
        /// This does not inherit from WFA because I like type safety. :-)
        class Xfa
        {
            wali::wfa::WFA wfa_;

            std::map<State, sem_elem_t> accepting_weights;            

        public:
            wali::wfa::WFA const &
            wfa() const {
                return wfa_;
            }
            
            /// Returns the old one
            ///
            State setInitialState(State new_initial) {
                return State(wfa_.setInitialState(new_initial.key));
            }

            State getInitialState() const {
                return State(wfa_.getInitialState());
            }

            void addFinalState(State final) {
                wfa_.addFinalState(final.key);
            }

            void addTrans(State src, Symbol sym, State tgt, sem_elem_t relation) {
                assert(relation.get_ptr());
                wfa_.addTrans(src.key, sym.key, tgt.key, relation);
            }

            virtual std::ostream & print( std::ostream & os) const {
                return wfa_.print(os);
            }

            virtual std::ostream& print_dot(std::ostream& o,
                                            bool print_weights=false,
                                            wali::wfa::DotAttributePrinter * printer=NULL) const
            {
                return wfa_.print_dot(o, print_weights, printer);
            }

            std::set<wali::Key> const & getFinalStateKeys() const {
                return wfa_.getFinalStates();
            }

            std::set<State> getFinalStates() const;

            std::set<wali::Key> const & getStateKeys() const {
                return wfa_.getStates();
            }

            void addState(State state, sem_elem_t relation) {
                assert(relation.get_ptr());
                wfa_.addState(state.key, relation);
            }

            wali::wfa::State const * getState(wali::Key name) const {
                return wfa_.getState(name);
            }

            void for_each( wali::wfa::ConstTransFunctor& tf ) const {
                wfa_.for_each(tf);
            }

            void for_each( wali::wfa::TransFunctor& tf ) {
                wfa_.for_each(tf);
            }

            bool isIsomorphicTo(Xfa const & other) {
                return wfa_.isIsomorphicTo(other.wfa_);
            }


            wali::wfa::WFA removeEpsilons() const {
                return wfa_.removeEpsilons();
            }

            void removeEpsilonsMutateThis() {
                wfa_ = wfa_.removeEpsilons();
            }

            void erase(State from, Symbol sym, State to) {
                wfa_.erase(from.key, sym.key, to.key);
            }

            /// TODO: change this to a vector of Symbols
            typedef std::vector<wali::Key> Word;            

            /// Returns whether the given string is accepted with a non-zero
            /// weight.
            ///
            /// This currently ignores the accepting weights -- a state is
            /// accepting or not.
            bool isAcceptedWithNonzeroWeight(Word const & word) const {
                return wfa_.isAcceptedWithNonzeroWeight(word);
            }

            /// Returns a WFA (unfortunately -- this should change TODO) that
            /// is semi-state-deterministic.
            ///
            /// @see wali::wfa::WFA::semideterminize
            Xfa
            semideterminize() const {
                assert(false);
                Xfa ret;
                ret.wfa_ = wfa_.semideterminize();
                return ret;
            }


            /// Returns a WFA (unfortunately -- this should change TODO) that
            /// is semi-state-deterministic.
            ///
            /// @see wali::wfa::WFA::semideterminize
            wali::wfa::WFA
            semideterminize(wali::wfa::DeterminizeWeightGen const & weight_gen) const {
                return wfa_.semideterminize(weight_gen);
            }


            /// Intersects this XFA with the other one and returns the result
            ///
            /// @see wali::wfa::WFA::intersect
            Xfa
            intersect(Xfa const & other) const {
                Xfa output;
                //details::IntersectionWeightMaker maker;
                this->wfa_.intersect(other.wfa_, output.wfa_);
                return output;
            }

            Xfa
            intersect(wali::wfa::WeightMaker & maker, Xfa const & other) const {
                Xfa output;
                this->wfa_.intersect(maker, other.wfa_, output.wfa_);
                return output;
            }

            Xfa
            intersect_conjoin(Xfa const & other) const {
                Xfa output;
                details::IntersectionWeightMaker maker;
                this->wfa_.intersect(maker, other.wfa_, output.wfa_);
                return output;
            }
        };
        

        /// Converts an NWA (from OpenNWA) into an Xfa. The NWA should only
        /// have internal transitions.
        ///
        /// All transitions in the resulting XFA have the identity relation
        /// (semiring 1) and all states have the nullary relation (semiring
        /// 0). 'rel' is unused except to get a handle on those.
        extern
        Xfa
        from_internal_only_nwa(opennwa::Nwa const & nwa,
                               sem_elem_t rel);



        /// Function object that will, given information about a transition
        /// in an automaton, "lift" that transition so that the new
        /// transformer also includes the current-state variable that
        /// respects the given transition.
        ///
        /// The values used for the current-state variable in the relation
        /// are [0,num-states) rather than the actual key values, which in
        /// most cases will allow it to be represented in fewer bits.
        struct IntroduceStateToRelationWeightGen
            : wali::wfa::LiftCombineWeightGen
        {
            /// Another "type safety" thing to help confusion between states
            /// numbered with Keys and states numbered starting from 0.
            struct SequentialFromZeroState {
                int index;
            
                explicit SequentialFromZeroState(int i)
                    : index(i)
                {}


                bool operator< (SequentialFromZeroState right) const {
                    return this->index < right.index;
                }
            };
        

            typedef boost::bimap<State, SequentialFromZeroState> SfzMap;

            /// This map tracks the correspondence between the wali Key value
            /// that's used for the state and the corresponding number in the
            /// range [0,num-states).
            SfzMap sequential_state_map;

            void set_up_sequential_state_map(Xfa const & xfa)
            {
                assert(sequential_state_map.size()==0);
                int next_sequential_state = 0;
                std::set<wali::Key> const & states = xfa.getStateKeys();
                for(std::set<wali::Key>::const_iterator state = states.begin();
                    state != states.end(); ++state)
                {
                    sequential_state_map.insert(SfzMap::value_type(State(*state),
                                                                   SequentialFromZeroState(next_sequential_state++)));
                }
            }


            State from_sfz_state(SequentialFromZeroState sfz_state) const
            {
                return sequential_state_map.right.at(sfz_state);
            }
            
            SequentialFromZeroState from_state(State state) const
            {
                return sequential_state_map.left.at(state);
            }
            

            
            wali::domains::binrel::ProgramBddContext const & new_voc;
            wali::domains::binrel::binrel_t havoc_current_state;

            /// The name (according to the ProgramBddContext) of the
            /// current-state variable for this XFA.
            std::string current_state;

            IntroduceStateToRelationWeightGen(wali::domains::binrel::ProgramBddContext const & v,
                                              Xfa const & xfa,
                                              std::string const & current_state_var_prefix)
                : new_voc(v)
                , current_state(current_state_var_prefix + "current_state")
            {
                set_up_sequential_state_map(xfa);

                using wali::domains::binrel::BinRel;
                havoc_current_state =
                    new BinRel(&v, new_voc.Assign(current_state, new_voc.NonDet()));
            }
                
            
            virtual sem_elem_t liftWeight(wali::wfa::WFA const & original_wfa,
                                          wali::Key source,
                                          wali::Key symbol,
                                          wali::Key target,
                                          sem_elem_t orig_weight) const;

            virtual sem_elem_t getOne(wali::wfa::WFA const & original_wfa) const;

            virtual
            sem_elem_t
            liftAcceptWeight(wali::wfa::WFA const & UNUSED_PARAMETER(original_wfa),
                             wali::Key state,
                             sem_elem_t original_accept_weight) const;
        };

        
        extern
        std::vector<std::map<std::string, int> >
        get_vars(XfaAst const & ast,
                 int fdd_size,
                 std::string const & prefix);

        extern
        Xfa
        from_parser_ast(RelationMaker const & maker,
                        XfaAst const & ast,
                        int fdd_size,
                        std::string const & domain_var_name_prefix);

        
        /// Pass an instance of this class to XFA::print_dot to get pretty
        /// diagrams.
        ///
        /// What it does is draw one of those dot-line diagrams for each
        /// transition, which illustrates the data transformer for that
        /// transition, and prints it out as an "onhover" attribute in the
        /// dot output.
        ///
        /// Using my dot-svg Python script [1], you can convert the dot
        /// output into an HTML file which, when you hover over a transition
        /// in the diagram, will display the dot-line diagram.
        ///
        /// [1] https://github.com/EvanED/dot-svg
        struct HoverWeightPrinter : wali::wfa::DotAttributePrinter {
            /// States do not get any extra attributes (though you could
            /// imagine printing out something!)
            virtual
            void
            print_extra_attributes(wali::wfa::State const * UNUSED_PARAMETER(s),
                                   std::ostream & UNUSED_PARAMETER(os)
                                   ) CPP11_OVERRIDE
            {}


            ///
            /// For transitions, output the "onhover" attribute.
            virtual
            void
            print_extra_attributes(const wali::wfa::ITrans* t,
                                   std::ostream & os
                                   ) CPP11_OVERRIDE;
        };
        
        bool
        language_contains(Xfa const & left, Xfa const & right,
                          wali::domains::binrel::ProgramBddContext const & voc);

    }
}

#endif
