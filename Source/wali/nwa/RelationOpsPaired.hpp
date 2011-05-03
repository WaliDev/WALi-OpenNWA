#ifndef RELATION_OPS_PAIRED_HPP
#define RELATION_OPS_PAIRED_HPP

#include <algorithm>
#include <iterator>
#include <cassert>
#include <utility>
#include <map>
#include <set>

#define relations std_set_relations
#include "RelationOps.hpp"
#undef relations
#define relations buddy_relations
#include "RelationOpsBuddy.hpp"
#undef relations

namespace wali {
  namespace relations {
    namespace wsr = wali::std_set_relations;
    namespace wbr = wali::buddy_relations;

    typedef wsr::RelationTypedefs<unsigned long>::BinaryRelation wsrbr;
    typedef wbr::RelationTypedefs<unsigned long>::BinaryRelation wbrbr;

    extern int count;
    inline
    void counter(char* vals, int size) {
      int numDontCares = 0;
      for (int i=0; i<size; ++i) {
        if (vals[i] < 0) {
          ++numDontCares;
        }
      }

      //for (int i=0; i<size; ++i) {
      //    std::cout << (vals[i] < 0 ? 'X' : ('0' + vals[i]));
      //}
      //std::cout << "\n";
            
      count += 1 << numDontCares;
    }

    inline
    int bdd_rel_size(::bdd b, int factor) {
      count = 0;
      //std::cout << "Call to allsat:\n";
      bdd_allsat(b, counter);

      assert(bdd_satcount(b) == count);
      return count/factor;
    }
        
    /// Wraps a bdd in a nice friendly package
    struct BinaryRelation {
      wsrbr set;
      wbrbr bdd;

      BinaryRelation(unsigned int largest)
        : bdd(largest)
      {}

      BinaryRelation()
      {}

      bool insert(unsigned int leftVal, unsigned int rightVal)
      {
        return insert(std::make_pair(leftVal, rightVal));
      }
            
      bool insert(std::pair<unsigned int, unsigned int> pair)
      {
        assert(check());
        bool added1 = set.insert(pair).second;
        bool added2 = bdd.insert(pair);
        assert(check());
        assert(added1 == added2);
        return added1;
      }
            
      bool empty() const
      {
        assert(set.empty() == bdd.empty());
        return set.empty();
      }
            
      bool operator== (BinaryRelation const & other) const {
        assert( (set == other.set) == (bdd == other.bdd) );
        return bdd == other.bdd;
      }

      ::bdd getBdd() const {
        return bdd.getBdd();
      }

      int bddSize() const {
        return bdd_rel_size(bdd.getBdd(), 256);
      }

      int setSize() const {
        return set.size();
      }

      bool check() const {
        return bddSize() == setSize();
      }
    };

    struct TernaryRelation {
      wsr::RelationTypedefs<unsigned long>::TernaryRelation set;
      wbr::RelationTypedefs<unsigned long>::TernaryRelation bdd;

      TernaryRelation(unsigned int largest)
        : bdd(largest)
      {}

      TernaryRelation()
      {}

      bool insert(unsigned int leftVal, unsigned int middleVal, unsigned int rightVal)
      {
        bool added1 = set.insert(Triple<unsigned long,unsigned long,unsigned long>(leftVal, middleVal, rightVal));
        bool added2 = bdd.insert(Triple<unsigned,unsigned,unsigned>(leftVal, middleVal, rightVal));
        assert(added1 == added2);
        return added1;
      }

      bool insert(Triple<unsigned, unsigned, unsigned> triple)
      {
        return insert(triple.first, triple.second, triple.third);
      }

      bool insert(Triple<unsigned long, unsigned long, unsigned long> triple)
      {
        return insert(triple.first, triple.second, triple.third);
      }

      bool check() const {
        return bdd_rel_size(bdd.getBdd(), 16) == set.size();
      }
    };

        
    /// This can be used in client code to hide the actual relation types
    template<typename State>
    struct RelationTypedefs
    {
      typedef wali::relations::BinaryRelation BinaryRelation;
      typedef wali::relations::TernaryRelation TernaryRelation;
    };

        
    using wbr::buddyInit;
        
    // inline
    // void buddyInit()
    // {
    //     if (!bdd_isrunning()) {
    //         const int million = 1000000;
    //         int rc = bdd_init( 50*million, 100000 );
    //         if( rc < 0 ) {
    //             std::cerr << "[ERROR] " << bdd_errstring(rc) << std::endl;
    //             assert( 0 );
    //             exit(10);
    //         }
    //         // Default is 50,000 (1 Mb),memory is cheap, so use 100,000
    //         bdd_setmaxincrease(100000);
    //     }
    // }


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
      wsr::compose<unsigned long>(out_result.set, r1.set, r2.set);
      wbr::compose(out_result.bdd, r1.bdd, r2.bdd);
      assert(out_result.check());
    }

        
    /// Projects out the symbol in the internal and call relation
    ///
    /// {(source, target) | (source, alpha, target) \in delta}
    ///
    /// Parameters:
    ///   out_result: The relation delta restricted to alpha
    ///   delta:      Internals or calls relation
    ///   alpha:      Alphabet symbol to select and project
    template<typename OutRelation, typename State, typename Symbol>
    void
    project_symbol_3_set(OutRelation & out_result,
                         std::set<Triple<State, Symbol, State> > const & delta,
                         Symbol alpha)
    {
      wsr::project_symbol_3<OutRelation, State, Symbol>(out_result, delta, alpha);
    }

        
    template<typename OutRelation, typename State, typename Symbol>
    void
    project_symbol_3(OutRelation & out_result,
                     std::set<Triple<State, Symbol, State> > const & delta,
                     Symbol alpha)
    {
      wsr::project_symbol_3<wsrbr, State, Symbol>(out_result.set, delta, alpha);
      wbr::project_symbol_3<wbrbr, State, Symbol>(out_result.bdd, delta, alpha);
      assert(out_result.check());
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
    inline void
    merge(BinaryRelation & out_result,
          BinaryRelation const & r_exit,
          BinaryRelation const & r_call,
          TernaryRelation const & delta_r)
    {
      assert(out_result.check());
      assert(r_exit.check());
      assert(r_call.check());
      assert(delta_r.check());
      wsr::merge<unsigned long>(out_result.set, r_exit.set, r_call.set, delta_r.set);
      wbr::merge(out_result.bdd, r_exit.bdd, r_call.bdd, delta_r.bdd);
      //std::cout << out_result.bdd.getBdd() << "\n";
      assert(out_result.check());
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
    project_symbol_4(TernaryRelation & out_result,
                     std::set<Quad<State, State, Symbol, State> > const & delta,
                     Symbol alpha)
    {
      assert(out_result.check());
      wsr::project_symbol_4<State, Symbol>(out_result.set, delta, alpha);
      wbr::project_symbol_4<State, Symbol>(out_result.bdd, delta, alpha);
      assert(out_result.check());
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
    template<typename Relation>
    void
    transitive_closure(Relation & out_result,
                       Relation const & r)
    {
      typedef typename Relation::const_iterator Iterator;
      typedef typename Relation::value_type::first_type State;

#ifndef NDEBUG
      const bool domain_and_codomain_are_the_same =
        boost::is_same<typename Relation::value_type::first_type,
        typename Relation::value_type::second_type>::value;
      BOOST_STATIC_ASSERT(domain_and_codomain_are_the_same);
#endif

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
            out_result.insert(std::make_pair(source,target));
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
    inline void
    intersect(BinaryRelation & out_result,
              BinaryRelation const & r1,
              BinaryRelation const & r2)
    {
      wsr::intersect(out_result.set, r1.set, r2.set);
      wbr::intersect(out_result.bdd, r1.bdd, r2.bdd);
      assert(out_result.check());
    }


    /// Returns the union of two binary relations on states
    ///
    /// Parameters:
    ///   out_result: The union of r1 and r2
    ///   r1:         One binary relation on states
    ///   r2:         Another binary relation on states
    inline void
    union_(BinaryRelation & out_result,
           BinaryRelation const & r1,
           BinaryRelation const & r2)
    {
      assert(r1.check());
      assert(r2.check());
      wsr::union_(out_result.set, r1.set, r2.set);
      wbr::union_(out_result.bdd, r1.bdd, r2.bdd);
      assert(out_result.check());
    }



    template<typename T>
    class VectorSet {
      typedef std::vector<T> Items;
      Items items;

    public:
      typedef typename Items::iterator iterator;
      typedef typename Items::const_iterator const_iterator;
           
      void insert(T const & item)  {
        if (find(item) == end()) {
          items.push_back(item);
        }
      }

      iterator find(T const & item) { return std::find(items.begin(), items.end(), item); }
      const_iterator find(T const & item) const { return std::find(items.begin(), items.end(), item); }

      bool empty() const { return items.empty(); }

      iterator begin() { return items.begin(); }
      const_iterator begin() const { return items.begin(); }

      iterator end() { return items.end(); }
      const_iterator end() const { return items.end(); }

      void erase(iterator i) { items.erase(i); }
      void erase(const_iterator i) { items.erase(i); }
    };
        
  } // namespace relations
} // namespace wali

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:

#endif
