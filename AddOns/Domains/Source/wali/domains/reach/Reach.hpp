#ifndef wali_domains_reach_REACH_GUARD
#define wali_domains_reach_REACH_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/SemElem.hpp"
#include "wali/MergeFn.hpp"
#include "wali/WeightFactory.hpp"
#include "wali/MergeFnFactory.hpp"
#include <string>


namespace wali
{
  namespace domains
  {
    namespace reach
    {
      class Reach : 
        public wali::SemElem,
        public wali::WeightFactory,
        public wali::MergeFnFactory
      {

        public:

          Reach( bool b );

          virtual ~Reach();

          wali::sem_elem_t one() const;

          wali::sem_elem_t zero() const;

          // zero is the annihilator for extend
          wali::sem_elem_t extend( wali::SemElem* rhs );

          // zero is neutral for combine
          wali::sem_elem_t combine( wali::SemElem* rhs );

          bool equal( wali::SemElem* rhs ) const;

          std::ostream & print( std::ostream & o ) const;

          // For WeightFactory
          virtual wali::sem_elem_t getWeight( std::string s );

          // For MergeFnFactory
          virtual wali::merge_fn_t getMergeFn( std::string s );

        public:
          static int numReaches;

        protected:
          bool isreached;

      }; // class Reach

    } // namespace reach

  } // namespace domains

} // namespace wali

#endif	// wali_domains_reach_REACH_GUARD

