#ifndef RELATION_OPS_HPP
#define RELATION_OPS_HPP

#include <algorithm>
#include <iterator>
#include <cassert>
#include <map>
#include <set>

#include <fdd.h>

#include "wali/ref_ptr.hpp"
#include "wali/KeyContainer.hpp"

namespace wali {
    namespace relations {

        /// Returns floor(log_2(n))
        //
        // E.g. floorLog2(11):
        //  iteration#   n (bin)     n (dec)   log [values at end of iteration]
        //     --         1011         11       -1
        //     1          0101         5        0
        //     2          0010         2        1
        //     3          0001         1        2
        //     4          0000         0        3
        //
        // log_2(11) ~= 3.46, so we're right (we return 3)
        //
        // E.g. floorLog2(4):
        //  iteration#   n (bin)     n (dec)   log [values at end of iteration]
        //     --         0100          4      -1
        //     1          0010          2      0
        //     2          0001          1      1
        //     3          0000          0      2
        //
        // log_2(4) = 2, so again we're right.
        inline int floorLog2(unsigned int n) {
            assert(n>0);

            int log = -1;
            while (n > 0) {
                n >>= 1;
                ++log;
            }
            return log;
        }


        /// Given two sizes, return the FDD id numbers that should be used for
        /// each component.
        //
        // It does this by looking up the given sizes (in bits) in a static map.
        // If we haven't created a set of FDD numbers for those particular sizes yet,
        // then create some.
        inline
        std::pair<int, int> getFddNumbers(unsigned int leftBits, unsigned int rightBits)
        {
            // Maps from a relation size to the base bdd number.
            static std::map<std::pair<unsigned int, unsigned int>, int> fddMap;

            std::pair<unsigned int, unsigned int> bits(leftBits, rightBits);

            if (fddMap.find(bits) == fddMap.end()) {
                // Make a new domain for each relation component
                int domains[2] = {
                    1 << leftBits - 1,  // 2^n - 1 is largest # representable in n bits
                    1 << rightBits - 1
                };
                base = fdd_extdomain(domains, 2);
                fddMap[bits] = base;
            }

            int base = fddMap[bits];

            return makepair(base, base+1);
        }


        /// Private structure
        //
        // Used for each component in a relation (e.g. domain & range)
        struct Component {
            int fdd_number;
            int size;
            int bits;

            Component(int size_) {
                size = size_;
                bits = floorLog2(size_) + 1;
                fdd_number = -1; // must be set elsewhere
            }
        };

        
        /// Wraps a bdd in a nice friendly package
        class BinaryRelation {        
            Component left;
            Component right;

            bdd myBdd;

        public:
            BinaryRelation(unsigned int leftSize, unsigned int rightSize)
                : left(leftSize)
                , right(rightSize)
                , myBdd(bddfalse)
            {
                // We have to get the FDD numbers later, because both depend on both
                pair<int, int> fdds = getFddNumbers(left.bits, right.bits);
                left.fdd_number = fdds.first;
                right.fdd_number = fdds.second;
            }


            void insert(unsigned int leftVal, unsigned int rightVal)
            {
                bdd left_is_leftVal = fdd_ithvar(left.fdd_number, leftVal);
                bdd right_is_rightVal = fdd_ithvar(right.fdd_number, rightVal);

                myBdd = myBdd | (left_is_leftVal & right_is_rightVal);
            }

            void insert(std::pair<unsigned int, unsigned int> pair)
            {
                insert(pair.first, pair.second);
            }

            friend void compose(BinaryRelation &, BinaryRelation const &, BinaryRelation const &);
        };

        

        /// Given two sizes, return the FDD id numbers that should be used for
        /// each component.
        //
        // It does this by looking up the given sizes (in bits) in a static map.
        // If we haven't created a set of FDD numbers for those particular sizes yet,
        // then create some.
        inline
        Triple<int, int, int> getFddNumbers(unsigned int leftBits,
                                            unsigned int middleBits,
                                            unsigned int rightBits)
        {
            typedef Triple<unsigned int, unsigned int, unsigned int> SizeTriple;
            
            // Maps from a relation size to the base bdd number.
            static std::map<SizeTriple, int> fddMap;
            SizeTriple bits(leftBits, middleBits, rightBits);

            if (fddMap.find(bits) == fddMap.end()) {
                // Make a new domain for each relation component
                int domains[3] = {
                    1 << leftBits - 1,  // 2^n - 1 is largest # representable in n bits
                    1 << middleBits - 1,
                    1 << rightBits - 1
                };
                base = fdd_extdomain(domains, 3);
                fddMap[bits] = base;
            }

            int base = fddMap[bits];

            return SizeTriple(base, base+1, base+2);
        }


        /// Wraps a bdd in a nice friendly package
        class TernaryRelation {
            Component left;
            Component middle;
            Component right;

            bdd myBdd;

        public:
            BinaryRelation(unsigned int leftSize, unsigned int middleSize, unsigned int rightSize)
                : left(leftSize)
                , middle(middleSize)
                , right(rightSize)
                , myBdd(bddfalse)
            {
                // We have to get the FDD numbers later, because both depend on both
                Triple<int, int, int> fdds = getFddNumbers(left.bits, middle.bits, right.bits);
                left.fdd_number = fdds.first;
                middle.fdd_number = fdds.second
                right.fdd_number = fdds.third;
            }


            void insert(unsigned int leftVal, unsigned int middleVal, unsigned int rightVal)
            {
                bdd left_is_leftVal = fdd_ithvar(left.fdd_number, leftVal);
                bdd middle_is_middleVal = fdd_ithvar(middle.fdd_number, middleVal);
                bdd right_is_rightVal = fdd_ithvar(right.fdd_number, rightVal);

                myBdd = myBdd | (left_is_leftVal & middle_is_middleVal & right_is_rightVal);
            }

            void insert(Triple<unsigned, unsigned, unsigned> triple)
            {
                insert(triple.first, triple.second, triple.third);
            }
        };

        
        /// This can be used in client code to hide the actual relation types
        template<typename State>
        struct RelationTypedefs
        {
            typedef bdd BinaryRelation;
            typedef bdd TernaryRelation;
        };


        void buddyInit()
        {
            const int million = 1000000;
            int rc = bdd_init( 50*million, 100000 );
            if( rc < 0 ) {
                std::cerr << "[ERROR] " << bdd_errstring(rc) << endl;
                assert( 0 );
                exit(10);
            }
            // Default is 50,000 (1 Mb),memory is cheap, so use 100,000
            bdd_setmaxincrease(100000);
        }


        /// Composes two binary relations
        ///
        /// { (x,z) | (x,y) \in r1,  (y,z) \in r2}
        ///
        /// Parameters:
        ///   out_result: The relational composition of r1 and r2
        ///   r1:         relation 1
        ///   r2:         relation 2
        inline void
        compose(BinaryRelation & out_result,
                BinaryRelation const & r1,
                BinaryRelation const & r2)
        {
            if (r1.right.fdd_number != r2.left.fdd_number) {
                std::cerr << "Error: compose (Buddy version): relations don't share an FDD\n";
                exit(20);
            }

            bdd r1_bdd = r1.myBdd;
            bdd r2_shifted = bdd_replace(that->rel, downOne);
                
            bdd_relprod(rel, ...,fdd_ithset(base+1));
        }

        /// Projects out the symbol in the internal and call relation
        ///
        /// {(source, target) | (source, alpha, target) \in delta}
        ///
        /// Parameters:
        ///   out_result: The relation delta restricted to alpha
        ///   delta:      Internals or calls relation
        ///   alpha:      Alphabet symbol to select and project
        template<typename State, typename Symbol>
        void
        project_symbol_3(typename RelationTypedefs<State>::BinaryRelation & out_result,
                         std::set<Triple<State, Symbol, State> > const & delta,
                         Symbol alpha)
        {
            typedef typename std::set<Triple<State, Symbol, State> >::const_iterator Iterator;

            for(Iterator cur_trans = delta.begin(); cur_trans != delta.end(); ++cur_trans)
                {
                    State source = cur_trans->first;
                    Symbol symb = cur_trans->second;
                    State target = cur_trans->third;

                    if(symb == alpha)
                        {
                            out_result.insert(make_pair(source, target));
                        }
                }
        }


        /// Performs the sort of merge required for NWA return edges
        ///
        /// {(q, q') | (q,q1) \in r_call, (q1,q2) \in r_exit, (q2,q1,q') \in delta}
        ///
        /// Parameters:
        ///   out_result: The relational composition of R1 and R2
        ///   r_exit:     The relation at the exit node
        ///   r_call:     The relation at the call node
        ///   delta_r:    The return transition relation with the alphabet
        ///               symbol projected out
        template<typename State>
        void
        merge(typename RelationTypedefs<State>::BinaryRelation & out_result,
              typename RelationTypedefs<State>::BinaryRelation const & r_exit,
              typename RelationTypedefs<State>::BinaryRelation const & r_call,
              typename RelationTypedefs<State>::TernaryRelation const & delta_r)
        {
            typedef typename RelationTypedefs<State>::BinaryRelation::const_iterator Iterator;

            typename RelationTypedefs<State>::BinaryRelation temp;

            // Test possibilites for (call_pred, exit) in r_exit
            for(Iterator exit_iter = r_exit.begin(); exit_iter != r_exit.end(); ++exit_iter)
                {
                    // Test possibilities for (exit, call_pred, return_) in delta
                    typedef typename RelationTypedefs<State>::TernaryRelation::const_iterator BigIterator;
                    pair<BigIterator, BigIterator> range =
                        delta_r.equal_range(make_pair(exit_iter->second, exit_iter->first));
        
                    for(BigIterator ret_trans = range.first; ret_trans != range.second; ++ret_trans) {
                        if(exit_iter->first == ret_trans->second && exit_iter->second == ret_trans->first) {
                            temp.insert(make_pair(exit_iter->first, ret_trans->third));
                        }
                    }
                }

            compose<State>(out_result, r_call, temp);
        }


        /// Projects out the symbol in the return relation
        ///
        /// {(source, pred, target) | (source, pred, alpha, target) \in delta}
        ///
        /// Parameters:
        ///   out_result: The ternary relation delta restricted to alpha
        ///   delta:      Return relation
        ///   alpha:      Alphabet symbol to select and project
        template<typename State, typename Symbol>
        void
        project_symbol_4(typename RelationTypedefs<State>::TernaryRelation & out_result,
                         std::set<Quad<State, State, Symbol, State> > const & delta,
                         Symbol alpha)
        {
            typedef typename set<Quad<State, State, Symbol, State> >::const_iterator Iterator;

            for(Iterator cur_trans = delta.begin(); cur_trans != delta.end(); ++cur_trans)
                {
                    State source = cur_trans->first;
                    State pred = cur_trans->second;
                    Symbol symb = cur_trans->third;
                    State target = cur_trans->fourth;

                    if(symb == alpha)
                        {
                            out_result.insert(Triple<State, State, State>(source, pred, target));
                        }
                }
        }


        template<typename State>
        State biggest(State s1, State s2, State s3)
        {
            return std::max(s1, std::max(s2, s3));
        }

        /// Constructs the transitive closure of an algorithm.
        ///
        /// TODO: Right now we break the abstraction and assume State is a
        /// (reasonably small) integer.
        ///
        /// { (p, q) | p R* q} or however you want to denote it
        ///
        /// Parameters:
        ///   out_result: The transitive closure of r
        ///   r:          The source relation
        template<typename State>
        void
        transitive_closure(typename RelationTypedefs<State>::BinaryRelation & out_result,
                           typename RelationTypedefs<State>::BinaryRelation const & r)
        {
            typedef typename RelationTypedefs<State>::BinaryRelation::const_iterator Iterator;

            // Find the largest state
            State largest_state = State();
            for(Iterator edge = r.begin(); edge != r.end(); ++edge)
                {
                    largest_state = biggest(largest_state, edge->first, edge->second);
                }

            largest_state = largest_state + 1;

            assert(largest_state < 2000); // reasonable based on my examples
            // I think
      
            // Set up the path matrix
            std::vector<std::deque<bool> > matrix(largest_state);
            for(size_t i = 0; i<matrix.size(); ++i)
                {
                    matrix[i].resize(largest_state);
                }

            for(size_t source=0; source<largest_state; ++source)
                {
                    matrix[source][source] = 1;
                }
       
            for(Iterator edge = r.begin(); edge != r.end(); ++edge)
                {
                    matrix[edge->first][edge->second] = 1;
                }

            // Now perform Floyd-Warshall alg. From Wikipedia:
            //
            //  /* Assume a function edgeCost(i,j) which returns the cost of
            //     the edge from i to j (infinity if there is none).  Also
            //     assume that n is the number of vertices and
            //     edgeCost(i,i) = 0
            //   */
            //
            //  int path[][];
            //
            //  /* A 2-dimensional matrix. At each step in the algorithm,
            //     path[i][j] is the shortest path from i to j using
            //     intermediate vertices (1..k-1).  Each path[i][j] is
            //     initialized to edgeCost(i,j) or infinity if there is no
            //     edge between i and j.
            //   */
            //
            //   procedure FloydWarshall ()
            //      for k := 1 to n
            //         for i := 1 to n
            //            for j := 1 to n
            //               path[i][j] = min ( path[i][j], path[i][k]+path[k][j] );

            for(size_t k = 0; k < largest_state; ++k)
                for(size_t i = 0; i < largest_state; ++i)
                    for(size_t j = 0; j < largest_state; ++j)
                        matrix[i][j] = matrix[i][j] || (matrix[i][k] && matrix[k][j]);


            // Now go through and convert back to the multimap view
            for(size_t source=0; source<largest_state; ++source)
                {
                    for(size_t target=0; target<largest_state; ++target)
                        {
                            if(matrix[source][target])
                                {
                                    out_result.insert(make_pair(source,target));
                                }
                        }
                } // done with 
        }


        /// Returns the intersection of two binary relations on states
        ///
        /// Parameters:
        ///   out_result: The intersection of r1 and r2
        ///   r1:         One binary relation on states
        ///   r2:         Another binary relation on states
        template<typename Relation>
        void
        intersect(Relation & out_result,
                  Relation const & r1,
                  Relation const & r2)
        {
            std::set_intersection(r1.begin(), r1.end(),
                                  r2.begin(), r2.end(),
                                  std::inserter(out_result, out_result.begin()));
        }


        /// Returns the union of two binary relations on states
        ///
        /// Parameters:
        ///   out_result: The union of r1 and r2
        ///   r1:         One binary relation on states
        ///   r2:         Another binary relation on states
        template<typename Relation>
        void
        union_(Relation & out_result,
               Relation const & r1,
               Relation const & r2)
        {
            std::set_union(r1.begin(), r1.end(),
                           r2.begin(), r2.end(),
                           std::inserter(out_result, out_result.begin()));
        }
    } // namespace relations
} // namespace wali


#endif
