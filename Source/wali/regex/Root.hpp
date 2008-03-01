#ifndef wali_regex_ROOT_GUARD
#define wali_regex_ROOT_GUARD 1

/*!
 * @author Nick Kidd
 */

#include <string>
#include "wali/regex/Regex.hpp"

namespace wali {

    namespace regex {

        class Root : public Regex {

            public:
                Root(const std::string& label);
                Root(const std::string& label, wali::sem_elem_t se);
                virtual ~Root();

                virtual bool isConstant() const; /*! @return true */

                virtual void to_mona_recurse(std::ostream& o, const std::string& prefix ) const;
                virtual std::ostream& print_recurse( std::ostream& o ) const;
                virtual std::ostream& write_dot_recurse( std::ostream& o ) const;
                virtual wali::sem_elem_t solve_recurse();

                // doesn't override virtual void reset_marks();
            public:
                std::string lbl;
        };

    } // namespace regex

} // namespace wali

#endif  // wali_regex_ROOT_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */

