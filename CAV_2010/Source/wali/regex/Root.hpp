#ifndef wali_regex_ROOT_GUARD
#define wali_regex_ROOT_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"

#include "wali/regex/Regex.hpp"

namespace wali 
{
  namespace regex 
  {
    class Root : public Regex 
    {
      public:
        static regex_t make(Key k, sem_elem_t se);
        static regex_t ID();
        static regex_t NIL();

      public:
        virtual ~Root();

        virtual bool isConstant() const; /*! @return true */

        virtual void to_mona_recurse(std::ostream& o, const std::string& prefix ) const;
        virtual std::ostream& print_recurse( std::ostream& o ) const;
        virtual std::ostream& write_dot_recurse( std::ostream& o ) const;
        virtual wali::sem_elem_t solve_recurse();

      private:
        Root(Key label);
        Root(Key label, wali::sem_elem_t se);

        // doesn't override virtual void reset_marks();
      public:
        const Key lbl;
    };

  } // namespace regex

} // namespace wali

#endif  // wali_regex_ROOT_GUARD

