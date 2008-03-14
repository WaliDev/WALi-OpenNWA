#ifndef wpds_ERULE_GUARD
#define wpds_ERULE_GUARD

/*
 * @author Akash Lal
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/ewpds/MergeFunction.hpp"

//
// begin namespace wali
//
namespace wali
{
  namespace wpds
  {
    class Config;

    namespace ewpds
    {

      class ERule;
      typedef ref_ptr<ERule> erule_t;

      /*! @class ERule
       *
       * @see Rule
       */

      class ERule : public Rule
      {

        public:
          friend class EWPDS;

        public: 

          /*!
           * @brief constructor
           */
          ERule( Config *f, Config *t, wali_key_t s2, sem_elem_t se, merge_fn_t fn = NULL );

          /*!
           * @brief destructor
           */
          ~ERule();

          /*!
           * @return Rule rhs second stack symbol or WALI_EPSILON if no such
           * stack sym
           */
          merge_fn_t merge_fn() const { return mf; }

          /*! @return reference to the extended Weight */
          sem_elem_t extended_weight() const;

          /*!
           *
           * @param o the std::ostream this is written to
           * @return std::ostream passed in
           *
           * @see Printable
           */
          std::ostream & print( std::ostream &o ) const;

          /*!
           * marshall will write a ERule to a std::ostream. This writes
           * the rule in XML form.
           *
           * @return std::ostream the marshalled rule was written to.
           */
          std::ostream & marshall( std::ostream & o ) const;

        private:
          merge_fn_t mf;
          // AL: Removing extended_se from the class. The underlying rule weight is
          // sufficient, and we don't want to maintain redundant information
          //sem_elem_t extended_se;

      }; // class ERule

    } // end namespace ewpds

  } // end namespace wpds

} // end namespace wali

#endif  // wpds_ERULE_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */
