#ifndef wali_domains_lh_intra_AHval_GUARD
#define wali_domains_lh_intra_AHval_GUARD 1

/**
 * @author Nicholas Kidd
 *
 * An Acquisition History is an abstraction of a
 * path in a program. For [k] locks, it keeps track
 * of the number of locks held at the end of the path,
 * along with, for each held lock [l], the locks
 * that are acquired after [l].
 */

#include "fdd.h"
#include <iostream>

namespace wali
{
  namespace domains
  {
    namespace lh
    {
      namespace intra
      {
        class AHval 
        {
          public:
            static const int MAX_LOCKS;
            static int get_lock_count();
            static bool is_initialized();
            static void initialize( int num_locks );

          public:
            AHval();
            AHval(const AHval& that);

            AHval acquire( int lock ) const;
            AHval release( int lock ) const;

            std::ostream& print( std::ostream& o ) const;

          private:
            static void check_lock(int lock);
            static bdd add_lock_to_set(bdd R, int setnum, int lock);
            static void printHandler(char* v, int size);

          private:
            AHval( bdd R );
            static int LOCKCNT;
            static int NUMVARS;
            static int POWERSET_SIZE;
            bdd fVal;

        }; // class AHval

      } // namespace intra

    } // namespace lh

  } // namespace domains

} // namespace wali


std::ostream& operator<<(std::ostream&, const wali::domains::lh::intra::AHval& ah);

#endif  // wali_domains_lh_intra_AHval_GUARD



