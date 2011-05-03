#ifndef RELATION_OPS_BUDDY_HPP
#define RELATION_OPS_BUDDY_HPP

#include <algorithm>
#include <iterator>
#include <cassert>
#include <utility>
#include <map>
#include <set>

#ifndef NDEBUG
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#endif
#include <boost/shared_ptr.hpp>

#include <fdd.h>

#include "wali/ref_ptr.hpp"
#include "wali/KeyContainer.hpp"

namespace wali {
  namespace relations {
#if 0
    (Not used right now, but could be slightly helpful)
        
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
#endif

    inline
    Quad<int, int, int, int>
    getFddNumbers(unsigned int largest)
    {
      // Maps from a relation largest to the base bdd number.
      static std::map<unsigned int, int> fddMap;
            
      if (fddMap.find(largest) == fddMap.end()) {
        // Make a new domain for each relation component that we'll ever need in composition
        int domains[4] = {
          largest + 1,
          largest + 1,
          largest + 1,
          largest + 1
        };
        int base = fdd_extdomain(domains, 4);
        fddMap[largest] = base;
      }

      int base = fddMap[largest];

      return Quad<int, int, int, int>(base, base+1, base+2, base+3);
    }


    /// Private structure
    //
    // Used for each component in a relation (e.g. domain & range).
    //
    // This used to hold more, but was kind of neutered. Still, I like the
    // way it makes accesses look, so I'll leave it.
    struct Component {
      int fdd_number;

      bool operator== (Component rhs) const {
        return fdd_number == rhs.fdd_number;
      }

      bool operator!= (Component rhs) const {
        return fdd_number != rhs.fdd_number;
      }
    };


    /// This class represents a particular domain (set) that a given relation is over.
    ///
    /// Relations on a domain A can only be composed, merged, etc. with
    /// other relations on the domain A.
    class Domain {
    public: // FIXME
      Component left, middle, right, extra;

    private: // REMOVE ONCE THE ABOVE IS FIXED
      unsigned int _largest;

      typedef void(*pairFreer)(bddPair*);
      boost::shared_ptr<bddPair> shift_LM_to_MR;
      boost::shared_ptr<bddPair> shift_R_to_M;
      boost::shared_ptr<bddPair> shift_LR_to_RE;
      boost::shared_ptr<bddPair> shift_E_to_M;

    public:
      Domain(unsigned int largest)
        : shift_LM_to_MR(bdd_newpair(), bdd_freepair)
        , shift_R_to_M(bdd_newpair(), bdd_freepair)
        , shift_LR_to_RE(bdd_newpair(), bdd_freepair)
        , shift_E_to_M(bdd_newpair(), bdd_freepair)
      {
        _largest = largest;

        if (largest > 0) {
          Quad<int, int, int, int> fdds = getFddNumbers(largest);
          left.fdd_number = fdds.first;
          middle.fdd_number = fdds.second;
          right.fdd_number = fdds.third;
          extra.fdd_number = fdds.fourth;
                    
          fdd_setpair(shift_LM_to_MR.get(), left.fdd_number, middle.fdd_number);
          fdd_setpair(shift_LM_to_MR.get(), middle.fdd_number, right.fdd_number);
                    
          fdd_setpair(shift_R_to_M.get(), right.fdd_number, middle.fdd_number);
                    
          fdd_setpair(shift_LR_to_RE.get(), left.fdd_number, right.fdd_number);
          fdd_setpair(shift_LR_to_RE.get(), right.fdd_number, extra.fdd_number);
                    
          fdd_setpair(shift_E_to_M.get(), extra.fdd_number, middle.fdd_number);
        }
      }
            
      bool operator!= (Domain const & rhs) const {
        return !(*this == rhs);
      }

      bool operator== (Domain const & rhs) const {
        if (_largest == rhs._largest) {
          assert (left == rhs.left
                  && middle == rhs.middle
                  && right == rhs.right
                  && extra == rhs.extra);
          return true;
        }
        else {
          return false;
        }
      }

      bddPair* shift_out_compose() const {
        return shift_LM_to_MR.get();
      }

      bddPair* shift_in_compose() const {
        return shift_R_to_M.get();
      }

      bddPair* shift_out_merge() const {
        return shift_LR_to_RE.get();
      }

      bddPair* shift_in_merge() const {
        return shift_E_to_M.get();
      }

      unsigned int largest() const {
        return _largest;
      }

    private:
      friend class BinaryRelation;
      friend class TernaryRelation;
    };

        
    /// Wraps a bdd in a nice friendly package
    class BinaryRelation {
    public: // FIXME
      Domain domain;
      bdd myBdd;

    public:
      BinaryRelation(unsigned int largest)
        : domain(largest)
        , myBdd(bddfalse)
      {}

      BinaryRelation()
        : domain(0)
        , myBdd(bddfalse)
      {}

      bool insert(unsigned int leftVal, unsigned int rightVal)
      {
        assert(leftVal <= domain.largest());
        assert(rightVal <= domain.largest());

        bdd left_is_leftVal = fdd_ithvar(domain.left.fdd_number, leftVal);
        bdd right_is_rightVal = fdd_ithvar(domain.middle.fdd_number, rightVal);

        bdd old = myBdd;
        myBdd = myBdd | (left_is_leftVal & right_is_rightVal);
        return myBdd != old;
      }

      bool insert(std::pair<unsigned int, unsigned int> pair)
      {
        return insert(pair.first, pair.second);
      }

      bool empty() const
      {
        return myBdd == bddfalse;
      }

      bdd getBdd() const
      {
        return myBdd;
      }

      bool operator== (BinaryRelation const & other) const {
        return (myBdd == other.myBdd && domain == other.domain);
      }

      friend void compose(BinaryRelation &, BinaryRelation const &, BinaryRelation const &);
      friend void intersect(BinaryRelation &, BinaryRelation const &, BinaryRelation const &);
      friend void union_(BinaryRelation &, BinaryRelation const &, BinaryRelation const &);
      friend void merge(BinaryRelation &, BinaryRelation const &, BinaryRelation const &, BinaryRelation const &);
    };


    /// Wraps a bdd in a nice friendly package
    class TernaryRelation {
    public: // FIXME
      Domain domain;
      bdd myBdd;

    public:
      TernaryRelation(unsigned int largest)
        : domain(largest)
        , myBdd(bddfalse)
      {}

      TernaryRelation()
        : domain(0)
        , myBdd(bddfalse)
      {}


      bool insert(unsigned int leftVal, unsigned int middleVal, unsigned int rightVal)
      {
        assert(leftVal <= domain.largest());
        assert(middleVal <= domain.largest());
        assert(rightVal <= domain.largest());

        bdd left_is_leftVal = fdd_ithvar(domain.left.fdd_number, leftVal);
        bdd middle_is_middleVal = fdd_ithvar(domain.middle.fdd_number, middleVal);
        bdd right_is_rightVal = fdd_ithvar(domain.right.fdd_number, rightVal);

        bdd old = myBdd;
        myBdd = myBdd | (left_is_leftVal & middle_is_middleVal & right_is_rightVal);
        return old != myBdd;
      }

      bool insert(Triple<unsigned, unsigned, unsigned> triple)
      {
        return insert(triple.first, triple.second, triple.third);
      }

      bool insert(Triple<unsigned long, unsigned long, unsigned long> triple)
      {
        return insert(triple.first, triple.second, triple.third);
      }

      bdd getBdd() const
      {
        return myBdd;
      }
    };

        
    /// This can be used in client code to hide the actual relation types
    template<typename State>
    struct RelationTypedefs
    {
      typedef wali::relations::BinaryRelation BinaryRelation;
      typedef wali::relations::TernaryRelation TernaryRelation;
    };


    inline
    void buddyInit()
    {
      if (!bdd_isrunning()) {
        const int million = 1000000;
        int rc = bdd_init( 50*million, 100000 );
        if( rc < 0 ) {
          std::cerr << "[ERROR] " << bdd_errstring(rc) << std::endl;
          assert( 0 );
          exit(10);
        }
        // Default is 50,000 (1 Mb),memory is cheap, so use 100,000
        bdd_setmaxincrease(100000);
      }
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
      if (r1.domain != r2.domain || out_result.domain != r1.domain) {
        std::cerr << "Error: compose (Buddy version): relations don't share a domain\n";
        exit(20);
      }

      bdd r1_bdd = r1.myBdd;
      bdd r2_shifted = bdd_replace(r2.myBdd, r2.domain.shift_out_compose());
      bdd composed = bdd_relprod(r1_bdd, r2_shifted, fdd_ithset(r1.domain.middle.fdd_number));
      out_result.myBdd = bdd_replace(composed, out_result.domain.shift_in_compose());
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
    project_symbol_3(OutRelation & out_result,
                     std::set<Triple<State, Symbol, State> > const & delta,
                     Symbol alpha)
    {
      typedef typename std::set<Triple<State, Symbol, State> >::const_iterator Iterator;

      for(Iterator cur_trans = delta.begin(); cur_trans != delta.end(); ++cur_trans) {
        State source = cur_trans->first;
        Symbol symb = cur_trans->second;
        State target = cur_trans->third;

        if(symb == alpha) {
          out_result.insert(std::make_pair(source, target));
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
    inline void
    merge(BinaryRelation & out_result,
          BinaryRelation const & r_exit,
          BinaryRelation const & r_call,
          TernaryRelation const & delta_r)
    {
      if (out_result.domain != r_exit.domain
          || r_exit.domain != r_call.domain
          || r_call.domain != delta_r.domain)
      {
        std::cerr << "Error: merge (Buddy version): relations don't share a domain\n";
        exit(20);
      }

      bdd r1_bdd = r_call.myBdd;
      bdd r2_bdd = bdd_replace(r_exit.myBdd, r_exit.domain.shift_out_compose());
      bdd r3_bdd = bdd_replace(delta_r.myBdd, r_call.domain.shift_out_merge());

      bdd middle_two = fdd_ithset(r_exit.domain.middle.fdd_number);
      //| fdd_ithset(r_exit.domain.right.fdd_number);

      bdd composed = bdd_appex(r1_bdd & r2_bdd,
                               r3_bdd,
                               bddop_and,
                               fdd_ithset(r_call.domain.middle.fdd_number));

      composed = bdd_exist(composed, fdd_ithset(r_exit.domain.right.fdd_number));
            
      out_result.myBdd = bdd_replace(composed, out_result.domain.shift_in_merge());
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
      typedef typename std::set<Quad<State, State, Symbol, State> >::const_iterator Iterator;

      for(Iterator cur_trans = delta.begin(); cur_trans != delta.end(); ++cur_trans)
      {
        State source = cur_trans->first;
        State pred = cur_trans->second;
        Symbol symb = cur_trans->third;
        State target = cur_trans->fourth;
                
        if(symb == alpha)
        {
          bool added = out_result.insert(Triple<State, State, State>(source, pred, target));
          assert(added);
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

      assert(largest_state < 4000); // reasonable based on my examples
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
      if (r1.domain != r2.domain || out_result.domain != r1.domain) {
        std::cerr << "Error: intersect (Buddy version): relations don't share a domain\n";
        exit(20);
      }

      out_result.myBdd = r1.myBdd & r2.myBdd;
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
      if (r1.domain != r2.domain || out_result.domain != r1.domain) {
        std::cerr << "Error: intersect (Buddy version): relations don't share a domain\n";
        exit(20);
      }

      out_result.myBdd = r1.myBdd | r2.myBdd;
    }



    template<typename T>
    class VectorSet {
      std::deque<T> items;

    public:
      typedef typename std::deque<T>::iterator iterator;
      typedef typename std::deque<T>::const_iterator const_iterator;
           
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
