#ifndef INCLUDE_CFGLIB_XFA_XFA_HPP
#define INCLUDE_CFGLIB_XFA_XFA_HPP

#include <boost/bimap.hpp>

#include <wali/wfa/WFA.hpp>
#include <wali/wfa/TransFunctor.hpp>
#include <wali/wfa/ITrans.hpp>
#include <wali/domains/binrel/BinRel.hpp>
#include <wali/domains/binrel/ProgramBddContext.hpp>
#include <wali/Key.hpp>

#include <opennwa/Nwa.hpp>

#include <map>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "../cpp11.hpp"
#include "base64.hpp"

namespace cfglib {
    namespace xfa {

        struct State {
            wali::Key key;
            
            explicit State(wali::Key k)
                : key(k)
            {}
        };

        struct SequentialFromZeroState {
            int index;
            
            explicit SequentialFromZeroState(int i)
                : index(i)
            {}
        };
        
        inline
        bool operator< (State left, State right) {
            return left.key < right.key;
        }

        inline
        bool operator< (SequentialFromZeroState left, SequentialFromZeroState right) {
            return left.index < right.index;
        }

        struct Symbol {
            wali::Key key;

            explicit Symbol(wali::Key k)
                : key(k)
            {}
        };

        inline
        State getState(std::string const & str) {
            return State(wali::getKey(str));
        }

        inline
        Symbol getSymbol(std::string const & str) {
            return Symbol(wali::getKey(str));
        }

        

        typedef wali::domains::binrel::binrel_t BinaryRelation;

        
        

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

            void erase(State from, Symbol sym, State to) {
                wfa_.erase(from.key, sym.key, to.key);
            }


            static
            std::vector<char>
            read_all(FILE* file) {
                std::vector<char> data;
                const int block_size = 4096;

                while (true) {
                    data.resize(data.size() + block_size);
                    int size_just_read = std::fread(&data[data.size()-block_size], 1u, block_size, file);

                    if (size_just_read < block_size) {
                        int excess = block_size - size_just_read;
                        data.resize(data.size() - excess);
                        return data;
                    }
                }
            }


            struct HoverWeightPrinter : wali::wfa::DotAttributePrinter {
                virtual void print_extra_attributes(wali::wfa::State const * s, std::ostream & os) CPP11_OVERRIDE {
                }
                
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

                    std::vector<char> image_data = read_all(image_data_stream);
                    std::vector<unsigned char> image_data_u(image_data.begin(), image_data.end());

                    std::string image_data_base64 = base64_encode(&image_data_u[0], image_data_u.size());

                    os << ",onhover=\"<img src='data:image/png;base64," << image_data_base64 << "'>\"";
                }
            };


            void output_draw_package(std::string const & filename) const {
                std::ofstream f(filename.c_str());
                HoverWeightPrinter p;
                this->print_dot(f, false, &p);
            }


            /// Returns whether the given string is accepted with a non-zero
            /// weight
            typedef std::vector<wali::Key> Word;            
            bool isAcceptedWithNonzeroWeight(Word const & word) const {
                return wfa_.isAcceptedWithNonzeroWeight(word);
            }


            wali::wfa::WFA semideterminize() const {
                return wfa_.semideterminize();
            }

            wali::wfa::WFA semideterminize(wali::wfa::DeterminizeWeightGen const & weight_gen) const {
                return wfa_.semideterminize(weight_gen);
            }
        };



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



        struct IntroduceStateToRelationWeightGen
            : wali::wfa::LiftCombineWeightGen
        {
            typedef boost::bimap<State, SequentialFromZeroState> SfzMap;
            SfzMap sequential_state_map;

            void set_up_sequential_state_map(Xfa const & xfa)
            {
                assert(sequential_state_map.size()==0);
                int next_sequential_state = 1;
                std::set<wali::Key> const & states = xfa.getStateKeys();
                for(std::set<wali::Key>::const_iterator state = states.begin();
                    state != states.end(); ++state)
                {
                    sequential_state_map.insert(SfzMap::value_type(State(*state),
                                                                   SequentialFromZeroState(next_sequential_state++)));
                }
            }

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
            

            
            wali::domains::binrel::ProgramBddContext new_voc;

            IntroduceStateToRelationWeightGen(wali::domains::binrel::ProgramBddContext v,
                                              Xfa const & xfa)
                : new_voc(v)
            {
                set_up_sequential_state_map(xfa);
            }
                
            
            virtual sem_elem_t liftWeight(wali::wfa::WFA const & original_wfa,
                                          wali::wfa::ITrans const * trans_in_original) const
            {
                //std::cout << "Lifting weight.\n";
                using wali::domains::binrel::BddContext;
                using wali::domains::binrel::BinRel;
                using wali::domains::binrel::bddinfo_t;

                // Zeroth step: havoc the current state
                BinaryRelation havoc_current_state =
                    new BinRel(&new_voc, new_voc.Assign("current_state", new_voc.NonDet()));

                sem_elem_t orig_rel_then_havoc =
                    trans_in_original->weight()->extend(havoc_current_state);

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
                wali::Key source = trans_in_original->from();
                wali::Key dest = trans_in_original->to();

                int source_fdd = safe_get(new_voc, "current_state")->baseLhs;
                int dest_fdd = safe_get(new_voc, "current_state")->baseRhs;

                //std::cout << "Creating state change BDD, setting\n"
                //            << "    FDD " << source_fdd << " to " << source << "\n"
                //          << "    FDD " << dest_fdd << " to " << dest << "\n";

                SequentialFromZeroState
                    sfz_source = from_state(State(source)),
                    sfz_dest = from_state(State(dest));

                bdd state_change = fdd_ithvar(source_fdd, sfz_source.index) & fdd_ithvar(dest_fdd, sfz_dest.index);

                //std::cout << "Created. Returning answer.\n";

                // Third step: combine them together
                return new BinRel(&new_voc, renamed_bdd & state_change);
            }
        };
        
    }
}

#endif
