#ifndef wali_regex_STAR_GUARD
#define wali_regex_STAR_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/regex/Regex.hpp"

namespace wali 
{
  namespace regex 
  {
    class Star : public Regex 
    {
      public:

        Star( regex_t child);

        virtual ~Star();

        virtual std::ostream& print_recurse( std::ostream& o ) const;

        virtual void to_mona_recurse(std::ostream& o, const std::string& prefix ) const;
        virtual std::ostream& write_dot_recurse( std::ostream& o ) const;
        virtual wali::sem_elem_t solve_recurse();

        virtual void reset_marks() const;
      public:
        regex_t child;
    };

  } // namespace regex

} // namespace wali

#endif  // wali_regex_STAR_GUARD

