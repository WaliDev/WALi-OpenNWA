#ifndef wali_regex_UNION_GUARD
#define wali_regex_UNION_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/regex/Regex.hpp"

#include <list>

namespace wali 
{
  namespace regex 
  {
    class Union : public Regex 
    {
      public:
        Union();

        virtual ~Union();

        void add(regex_t r);

        virtual std::ostream& print_recurse( std::ostream& o ) const;

        virtual void to_mona_recurse(std::ostream& o, const std::string& prefix ) const;
        virtual std::ostream& write_dot_recurse( std::ostream& o ) const;
        virtual wali::sem_elem_t solve_recurse();

        virtual void reset_marks() const;
      public:
        std::list<regex_t> children;
    };

  } // namespace regex 

} // namespace wali 

#endif  // wali_regex_UNION_GUARD

