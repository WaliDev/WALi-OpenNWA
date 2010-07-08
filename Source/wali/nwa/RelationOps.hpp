#ifndef RELATION_OPS_HPP
#define RELATION_OPS_HPP

#include <algorithm>
#include <iterator>
#include <map>
#include <set>

#include "wali/ref_ptr.hpp"
#include "wali/KeyContainer.hpp"

namespace wali {
  namespace relations {
    using std::set;
    using std::map;
    using std::pair;
    using std::make_pair;


    /// This class represents a ternary relation over the set 'Subject'
    template<typename Subject>
    class TernaryRelation
    {
      map<pair<Subject, Subject>, set<Subject> > relation2_1;

      typedef map<pair<Subject, Subject>, set<Subject> > Map;

    public:
      /// The iterator for this class
      class Iterator
      {
      public:
        typedef Triple<Subject, Subject, Subject> Tuple;

      private:
        typedef typename Map::const_iterator MapIter;
        typedef typename set<Subject>::const_iterator SetIter;

        // This holds the position in the outer set
        MapIter map_iter;
        MapIter end_map_iter;

        // This holds the position in the inner set
        SetIter set_iter;
        SetIter end_set_iter;

        Tuple cur_tuple;

        void set_tuple() {
          assert(!at_end());
          assert(set_iter != end_set_iter);
          assert(end_set_iter == map_iter->second.end());
          
          cur_tuple.first = map_iter->first.first;
          cur_tuple.second = map_iter->first.second;
          cur_tuple.third = *set_iter;
        }

      public:
        /// Initializes the iterator so it will walk over the outer map
        /// [start_outer, end_outer).
        Iterator(MapIter start_outer, MapIter end_outer) {
          map_iter = start_outer;
          end_map_iter = end_outer;

          if (map_iter != end_map_iter) {
            set_iter = map_iter->second.begin();
            end_set_iter = map_iter->second.end();
          }
        }

        /// Creates an iterator that walks over the given relation
        static Iterator begin(Map & relation) {
          return Iterator(relation.begin(), relation.end());
        }

        static Iterator end(Map & relation) {
          return Iterator(relation.end(), relation.end());
        }

        /// Returns whether the iterator is at the end of the relation
        bool at_end() const {
          return (map_iter == end_map_iter);
        }

        /// Advance the iterator.
        ///
        /// This function is void because I strongly disapprove of
        /// using '++' (either form) in the midst of another
        /// expression anyway. :-)
        void operator++() {
          assert(!at_end());
          assert(set_iter != end_set_iter);
          assert(end_set_iter == map_iter->second.end());

          // Advance the innermost iterator. If that brings us to the
          // end of the set, then go to the next set. If *that* brings
          // us to the end of the map (i.e. we were already on the
          // last set), then we're done, otherwise we need to say
          // we're working on the new set.
          ++set_iter;
          if(set_iter == end_set_iter) {
            advance_map();
          }
        }
          
        /// Avance the iterator
        void operator++(int) {
          this->operator++();
        }


        /// Return the tuple under the iterator
        Tuple const & operator*() const {
          assert(!at_end());
          set_tuple();
          return cur_tuple;
        }

        /// Returns the tuple under the iterator
        Tuple const * operator->() const {
          assert(!at_end());
          const_cast<Iterator*>(this)->set_tuple();
          return &cur_tuple;
        }

        /// Returns if two iterators are th same
        bool operator==(Iterator other) const {
          // Two iterators are the same if both are pointing to the
          // same element of the outer map AND either:
          //   1. both point at the same element of the set
          //   2. both are at the end of the map
          return (map_iter == other.map_iter
                  && (map_iter == end_map_iter
                      || set_iter == other.set_iter));
        }
        
        /// Returns if two iterators are different
        bool operator!=(Iterator other) const {
          return !(*this == other);
        }

        /// a
        void advance_map() {
          assert(!at_end());
          ++map_iter;
          if(map_iter != end_map_iter) {
            set_iter = map_iter->second.begin();
            end_set_iter = map_iter->second.end();
          }
        }
      };

    public:
      typedef Iterator iterator;
      typedef Iterator const_iterator;
      
      void insert(Triple<Subject, Subject, Subject> const & tuple) {
        relation2_1[make_pair(tuple.first, tuple.second)].insert(tuple.third);
      }

      iterator begin() { return Iterator::begin(relation2_1); }
      const_iterator begin() const { return Iterator::begin(const_cast<Map&>(relation2_1)); }
      iterator end() { return Iterator::end(relation2_1); }
      const_iterator end() const { return Iterator::end(const_cast<Map&>(relation2_1)); }

      /// Returns an iterator range that runs over all tuples where
      /// the first two coordinates match 'key'.
      pair<iterator, iterator> equal_range(pair<Subject, Subject> key) const {
        // The range starts at the beginning of the set related to key
        // and runs to the beginning of the set following that.
        typename Map::const_iterator store_iter = relation2_1.find(key);
        typename Map::const_iterator next_map = store_iter;

        // Now, if key wasn't actually *in* the map, then we want the
        // range to be empty. Otherwise, advance to the next set.
        if(store_iter->first == key) {
          ++next_map;
        }

        Iterator start(store_iter, next_map);
        Iterator end(next_map, next_map);

        return make_pair(start, end);
      }
    };


    /// This can be used in client code to hide the actual relation types
    template<typename State>
    struct RelationTypedefs
    {
      typedef set<pair<State, State> > BinaryRelation;
      typedef TernaryRelation<State> TernaryRelation;
    };


    /// Composes two binary relations
    ///
    /// { (x,z) | (x,y) \in r1,  (y,z) \in r2}
    ///
    /// Parameters:
    ///   out_result: The relational composition of r1 and r2
    ///   r1:         relation 1
    ///   r2:         relation 2
    template<typename State>
    void
    compose(typename RelationTypedefs<State>::BinaryRelation & out_result,
            typename RelationTypedefs<State>::BinaryRelation const & r1,
            typename RelationTypedefs<State>::BinaryRelation const & r2)
    {
      typedef typename set<pair<State, State> >::const_iterator Iterator;

      std::multimap<State, State> r2_map(r2.begin(), r2.end());
      typedef typename std::multimap<State, State>::const_iterator BigIterator;

      // Test possibilites for (origin, call_pred) \in r1
      for(Iterator r1_iter = r1.begin(); r1_iter != r1.end(); ++r1_iter)
      {
        State x = r1_iter->first;
        State y = r1_iter->second;

        // Test possibilites for (y, z) in r2
        pair<BigIterator, BigIterator> range = r2_map.equal_range(y);
        for(BigIterator r2_iter = range.first; r2_iter != range.second; ++r2_iter)
        {
          //if(r2_iter->first == y) {
            out_result.insert(make_pair(x, r2_iter->second));
          //}
        }
      }
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
      typedef typename set<pair<State, State> >::const_iterator Iterator;

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
      typedef typename set<pair<State, State> >::const_iterator Iterator;

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
    template<typename State>
    void
    intersect(set<pair<State, State> > & out_result,
	      set<pair<State, State> > const & r1,
	      set<pair<State, State> > const & r2)
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
    template<typename State>
    void
    union_(set<pair<State, State> > & out_result,
           set<pair<State, State> > const & r1,
           set<pair<State, State> > const & r2)
    {
      std::set_union(r1.begin(), r1.end(),
                     r2.begin(), r2.end(),
                     std::inserter(out_result, out_result.begin()));
    }
  } // namespace relations
} // namespace wali


#endif
