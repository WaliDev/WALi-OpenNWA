#ifndef wali_graph_GRAPH_COMMON_H_
#define wali_graph_GRAPH_COMMON_H_

#include "wali/Common.hpp"
#include "wali/wfa/ITrans.hpp"

namespace wali {

    namespace graph {


        struct Transition {
            Key src, tgt, stack;
            Transition() : src(0), tgt(0), stack(0) {}
            Transition(Key s, Key st, Key t) : src(s), tgt(t), stack(st) {}
            Transition(const Transition &t) : src(t.src), tgt(t.tgt), stack(t.stack) {}
            Transition(const wali::wfa::ITrans& t ) : src(t.from()), tgt(t.to()), stack(t.stack()) {}
            bool operator == (const Transition &t) {
                return (src == t.src && tgt == t.tgt && stack == t.stack);
            }    
        };

        struct TransitionCmp {
            bool operator() (const Transition &t1, const Transition &t2) const { // some total order on Transitions
                return ((t1.stack < t2.stack) ? true :
                        (t1.stack > t2.stack) ? false :
                        (t1.tgt < t2.tgt) ? true :
                        (t1.tgt > t2.tgt) ? false :
                        (t1.src < t2.src) ? true :
                        false);
            }
        };

        struct TransitionEq {
            bool operator() (const Transition &t1, const Transition &t2) const { // equality
                return (t1.stack == t2.stack && t1.src == t2.src && t1.tgt == t2.tgt);
            }
        };

        struct TransitionHash {
            size_t operator() (const Transition &t) const {
                return (997*t.stack + 131*t.tgt + t.src);
            }
        };

        typedef std::pair<Transition, wali::sem_elem_t> WTransition;


    } // namespace graph

} // namespace wali

#ifdef _WIN32
#define WIN(stmt) do { if(1) { stmt; } }while(0)
#else
#define WIN(stmt) 
#endif

#ifdef DEBUG
#define FWPDSDBGS(stmt) do { if(1) { stmt; } }while(0)
#else
#define FWPDSDBGS(stmt)
#endif

#ifdef FWPDS_INCREMENTAL
#define INCREMENTAL(stmt) do { if(1) { stmt; } }while(0)
#else
#define INCREMENTAL(stmt)
#endif

#ifdef FWPDS_STATS 
#define STAT(stmt) do { if(1) { stmt; } }while(0)
#else
#define STAT(stmt)
#endif

#endif // wali_graph_GRAPH_COMMON_H_
