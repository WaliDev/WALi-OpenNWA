#ifndef INCLUDE_CFGLIB_XFA_XFA_HPP
#define INCLUDE_CFGLIB_XFA_XFA_HPP

#include <wali/wfa/WFA.hpp>
#include <wali/wfa/TransFunctor.hpp>
#include <wali/wfa/ITrans.hpp>
#include <wali/domains/binrel/BinRel.hpp>
#include <wali/Key.hpp>

#include <map>
#include <sstream>
#include <fstream>
#include <cstdio>

#include "../cpp11.hpp"

namespace cfglib {
    namespace xfa {

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
                wfa_.addTrans(src.key, sym.key, tgt.key, relation);
            }

            virtual std::ostream & print( std::ostream & os) const {
                return wfa_.print(os);
            }

            virtual std::ostream& print_dot(std::ostream& o,
                                            bool print_weights=false ) const
            {
                return wfa_.print_dot(o, print_weights);
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


            struct Drawer : wali::wfa::ConstTransFunctor {
                std::string const & dir;
                Drawer(std::string const & d) : dir(d) {}
                
                virtual void operator()( const wali::wfa::ITrans* t ) {
                    wali::sem_elem_t w_se = t->weight();
                    BinaryRelation w = dynamic_cast<wali::domains::binrel::BinRel*>(w_se.get_ptr());
                    assert(w != NULL);

                    std::stringstream ss;
                    ss << dir << "/" << t->from() << "--" << t->to();
                    std::ofstream file((ss.str() + ".png").c_str());

                    std::stringstream command;

                    printImagemagickInstructions(w->getBdd(),
                                                 w->getVocabulary(),
                                                 command,
                                                 "png:-");

                    FILE* image_data_stream = popen(command.str().c_str(), "r");

                    std::vector<char> image_data = read_all(image_data_stream);

                    file.write(&image_data[0], image_data.size());
                }
            };


            void output_draw_package(std::string const & dirname) const {
                system(("rm -rf " + dirname).c_str());
                system(("mkdir " + dirname).c_str());
                
                std::ofstream f((dirname + "/fa.dot").c_str());
                this->print_dot(f);
                
                std::cout << "dot -Tsvg -o" << dirname << "/fa.svg " << dirname << "/fa.dot\n";
                std::cout << "cp ~/public/html/demo/fa.html " << dirname << "\n";
                
                Drawer d(dirname);
                wfa_.for_each(d);
            }


            /// Returns whether the given string is accepted with a non-zero
            /// weight
            typedef std::vector<wali::Key> Word;            
            bool isAcceptedWithNonzeroWeight(Word const & word) const {
                return wfa_.isAcceptedWithNonzeroWeight(word);
            }
            
        };
        
    }
}

#endif
