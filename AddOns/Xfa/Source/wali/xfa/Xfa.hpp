#ifndef INCLUDE_WALI_XFA_XFA_HPP
#define INCLUDE_WALI_XFA_XFA_HPP

#include <boost/bimap.hpp>

#include "ast.hpp"

#include <wali/wfa/WFA.hpp>
#include <wali/wfa/TransFunctor.hpp>
#include <wali/wfa/ITrans.hpp>
#include <wali/domains/binrel/BinRel.hpp>
#include <wali/domains/binrel/ProgramBddContext.hpp>
#include <wali/Key.hpp>
#include <wali/util/read_file.hpp>
#include <wali/util/base64.hpp>

#include <opennwa/Nwa.hpp>

#include <map>
#include <sstream>
#include <fstream>
#include <cstdio>


#define CPP11_OVERRIDE


namespace wali {
    namespace xfa {

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

        
        /// This is meant to abstract the representation of a relation so
        /// it's not tied to BDDs. I'm not sure how successful it is.
        typedef wali::domains::binrel::binrel_t BinaryRelation;

        
        
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

            std::map<State, BinaryRelation> accepting_weights;            

        public:
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

            void addTrans(State src, Symbol sym, State tgt, BinaryRelation relation) {
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

            std::set<State> getFinalStates() const {
                std::set<State> result;
                std::set<wali::Key> const & finals = getFinalStateKeys();
                for (std::set<wali::Key>::const_iterator final_key = finals.begin();
                     final_key != finals.end(); ++final_key)
                {
                    result.insert(State(*final_key));
                }
                return result;
            }

            std::set<wali::Key> const & getStateKeys() const {
                return wfa_.getStates();
            }

            void addState(State state, BinaryRelation relation) {
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


            /// Pass an instance of this class to XFA::print_dot to get
            /// pretty diagrams.
            ///
            /// What it does is draw one of those dot-line diagrams for each
            /// transition, which illustrates the data transformer for that
            /// transition, and prints it out as an "onhover" attribute in
            /// the dot output.
            ///
            /// Using my dot-svg Python script [1], you can convert the dot
            /// output into an HTML file which, when you hover over a
            /// transition in the diagram, will display the dot-line diagram.
            ///
            /// [1] https://github.com/EvanED/dot-svg
            struct HoverWeightPrinter : wali::wfa::DotAttributePrinter {
                /// States do not get any extra attributes (though you could
                /// imagine printing out something!)
                virtual void print_extra_attributes(wali::wfa::State const * s, std::ostream & os) CPP11_OVERRIDE {
                }


                ///
                /// For transitions, output the "onhover" attribute.
                virtual void print_extra_attributes( const wali::wfa::ITrans* t, std::ostream & os ) CPP11_OVERRIDE {
                    wali::sem_elem_t w_se = t->weight();
                    BinaryRelation w = dynamic_cast<wali::domains::binrel::BinRel*>(w_se.get_ptr());
                    assert(w != NULL);

                    std::stringstream command;

                    printImagemagickInstructions(w->getBdd(),
                                                 w->getVocabulary(),
                                                 command,
                                                 "png:-");

                    FILE* image_data_stream = popen(command.str().c_str(), "r");

                    if (image_data_stream == NULL) {
                        std::cerr << "Error opening pipe to " << command << ": ";
                        perror(NULL);
                    }

                    std::vector<char> image_data = util::read_file(image_data_stream);
                    pclose(image_data_stream);
                    
                    std::vector<unsigned char> image_data_u(image_data.begin(), image_data.end());

                    std::string image_data_base64 = util::base64_encode(&image_data_u[0], image_data_u.size());

                    os << ",onhover=\"<img src='data:image/png;base64," << image_data_base64 << "'>\"";
                }
            };


            /// Outputs a dot description of the XFA to the given file name,
            /// including the onhover dot-line transformer diagrams.
            ///
            /// (The name of this function is outdated, from a time when it
            /// would output the images in separate files.)
            void output_draw_package(std::string const & filename) const {
                std::ofstream f(filename.c_str());
                HoverWeightPrinter p;
                this->print_dot(f, false, &p);
            }


            /// Returns whether the given string is accepted with a non-zero
            /// weight.
            ///
            /// This currently ignores the accepting weights -- a state is
            /// accepting or not.
            typedef std::vector<wali::Key> Word;            
            bool isAcceptedWithNonzeroWeight(Word const & word) const {
                return wfa_.isAcceptedWithNonzeroWeight(word);
            }


            /// Returns a WFA (unfortunately -- this should change TODO) that
            /// is semi-state-deterministic.
            ///
            /// @see wali::wfa::WFA::semideterminize
            wali::wfa::WFA semideterminize() const {
                return wfa_.semideterminize();
            }


            /// Returns a WFA (unfortunately -- this should change TODO) that
            /// is semi-state-deterministic.
            ///
            /// @see wali::wfa::WFA::semideterminize
            wali::wfa::WFA semideterminize(wali::wfa::DeterminizeWeightGen const & weight_gen) const {
                return wfa_.semideterminize(weight_gen);
            }
        };



        /// Converts an NWA (from OpenNWA) into an Xfa. The NWA should only
        /// have internal transitions.
        ///
        /// All transitions in the resulting XFA have the identity relation
        /// (semiring 1) and all states have the nullary relation (semiring
        /// 0).
        inline
        Xfa from_internal_only_nwa(opennwa::Nwa const & nwa,
                                   BinaryRelation rel)
        {
            assert(rel.get_ptr());

            using opennwa::Nwa;
            using wali::domains::binrel::BinRel;
            Xfa xfa;

            BinaryRelation one = dynamic_cast<BinRel*>(rel->one().get_ptr());
            BinaryRelation zero = dynamic_cast<BinRel*>(rel->zero().get_ptr());

            assert(one.get_ptr());
            assert(zero.get_ptr());

            for (Nwa::StateIterator state = nwa.beginStates();
                 state != nwa.endStates() ; ++state)
            {
                xfa.addState(State(*state), zero);
            }

            assert(nwa.sizeInitialStates() == 1);
            xfa.setInitialState(State(*nwa.beginInitialStates()));

            for (Nwa::StateIterator state = nwa.beginFinalStates();
                 state != nwa.endFinalStates() ; ++state)
            {
                xfa.addFinalState(State(*state));
            }

            for (Nwa::InternalIterator trans = nwa.beginInternalTrans();
                 trans != nwa.endInternalTrans() ; ++trans)
            {
                xfa.addTrans(State(trans->first),
                             Symbol(trans->second),
                             State(trans->third),
                             one);
            }

            assert(nwa.sizeCallTrans() == 0);
            assert(nwa.sizeReturnTrans() == 0);

            return xfa;
        }


        /// Function object that, given two binary relations as weights,
        /// returns the and/intersection of them.
        ///
        /// Useful for intersection.
        struct AndWeightMaker
            : wali::wfa::WeightMaker
        {
            virtual sem_elem_t make_weight( sem_elem_t lhs, sem_elem_t rhs ) {
                wali::domains::binrel::BinRel
                    * l_rel = dynamic_cast<wali::domains::binrel::BinRel*>(lhs.get_ptr()),
                    * r_rel = dynamic_cast<wali::domains::binrel::BinRel*>(rhs.get_ptr());
                assert(l_rel && r_rel);

                wali::domains::binrel::ProgramBddContext
                    const & ctx = dynamic_cast<wali::domains::binrel::ProgramBddContext const &>(l_rel->getVocabulary());

                auto both = ctx.And(l_rel->getBdd(), r_rel->getBdd());
                return new wali::domains::binrel::BinRel(&ctx, both);
            }
        }; // WeightMaker
        

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


            /// Given a map 'm' and key 'k', return 'm[k]', except that it
            /// works when 'm' is const and it will abort() when 'k' is not
            /// present.
            template<typename Mapping>
            typename Mapping::mapped_type
            safe_get(Mapping const & m, typename Mapping::key_type const & k) const
            {
                typename Mapping::const_iterator it = m.find(k);
                if (it == m.end()) {
                    assert(false);
                }
                return it->second;
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
            BinaryRelation havoc_current_state;

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
                                          sem_elem_t orig_weight) const
            {
                //std::cout << "Lifting weight.\n";
                using wali::domains::binrel::BddContext;
                using wali::domains::binrel::BinRel;
                using wali::domains::binrel::bddinfo_t;

                // Zeroth step: havoc the current state
                sem_elem_t orig_rel_then_havoc =
                    orig_weight->extend(havoc_current_state);

                // not really the *original* relation, but close enough
                BinaryRelation orig_rel
                    = dynamic_cast<BinRel*>(orig_rel_then_havoc.get_ptr());

                // Now pull out the BDD and vocabulary
                bdd orig_bdd = orig_rel->getBdd();
                BddContext const & orig_voc = orig_rel->getVocabulary();

                // First step: rename variables to new domain
                std::vector<int> orig_names, new_names;

                for (BddContext::const_iterator var=orig_voc.begin(); var!=orig_voc.end(); ++var) {
                    bddinfo_t
                        orig_info = var->second,
                        new_info = safe_get(new_voc, var->first);

                    assert(new_info.get_ptr());

                    //std::cout << "Will rename: " << orig_info->baseLhs << "->" << new_info->baseLhs << "\n";
                    //std::cout << "Will rename: " << orig_info->baseRhs << "->" << new_info->baseRhs << "\n";

                    orig_names.push_back(orig_info->baseLhs);
                    orig_names.push_back(orig_info->baseRhs);
                    new_names.push_back(new_info->baseLhs);
                    new_names.push_back(new_info->baseRhs);

                }

                bddPair* rename_pairs = bdd_newpair();
                assert(orig_names.size() == new_names.size());
                fdd_setpairs(rename_pairs,
                             &orig_names[0],
                             &new_names[0],
                             orig_names.size());

                bdd renamed_bdd = bdd_replace(orig_bdd, rename_pairs);

                bdd_freepair(rename_pairs);

                // Second step: create a BDD that enforces that the state
                // changes in the right way.
                int source_fdd = safe_get(new_voc, current_state)->baseLhs;
                int dest_fdd = safe_get(new_voc, current_state)->baseRhs;

                SequentialFromZeroState
                    sfz_source = from_state(State(source)),
                    sfz_dest = from_state(State(target));

                //std::cout << "Creating state change BDD, setting\n"
                //          << "    FDD " << source_fdd << " to " << source << " (really " << sfz_source.index << ")\n"
                //          << "    FDD " << dest_fdd << " to " << dest << " (really " << sfz_dest.index << ")\n";

                bdd state_change = fdd_ithvar(source_fdd, sfz_source.index) & fdd_ithvar(dest_fdd, sfz_dest.index);

                //std::cout << "Created. Returning answer.\n";

                // Third step: combine them together
                return new BinRel(&new_voc, renamed_bdd & state_change);
            }
        };
        
        extern Xfa
        from_parser_ast(xfa_parser::Xfa const & ast,
                        wali::domains::binrel::ProgramBddContext & voc,
                        int fdd_size,
                        std::string const & domain_var_name_prefix);
    }
}

#endif
