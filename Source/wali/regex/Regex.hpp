#ifndef wali_regex_RE_GUARD
#define wali_regex_RE_GUARD 1

/*!
 * @author Nicholas Kidd
 */

#include "wali/Markable.hpp"
#include "wali/SemElem.hpp"

namespace wali 
{
  namespace regex 
  {
    class Regex;

    typedef wali::ref_ptr<Regex> regex_t;

    class Regex :
      public wali::Markable,
      public wali::SemElem
    {

      public:
        static const std::string lblID;
        static const std::string lblNIL;

        /*! @return regex_t == Root(lblID) */
        static regex_t ID();

        /*! @return regex_t == Root(lblNIL) */
        static regex_t NIL();
        static regex_t COMBINE( regex_t lhs, regex_t rhs );
        static regex_t EXTEND( regex_t lhs, regex_t rhs );
        static regex_t STAR( regex_t r );
      public:
        Regex();

        Regex(wali::sem_elem_t se);

        virtual ~Regex();

        // These three methods wrap calls to recursive method and the
        // calls reset_marks
        void toMona(std::ostream& o, const std::string& prefix ) const;
        std::ostream& write_dot( std::ostream& o ) const;
        wali::sem_elem_t solve();
        virtual void reset_marks() const;


        // Helper methods used in Spec::basicRegex
        bool isOne() const;
        bool isZero() const;
        virtual bool isConstant() const;

        // wali::SemElem methods
        virtual wali::sem_elem_t one() const;
        virtual wali::sem_elem_t zero() const;
        virtual bool equal( wali::SemElem * se ) const;
        virtual wali::sem_elem_t combine( wali::SemElem* se );
        virtual wali::sem_elem_t extend( wali::SemElem* se );
        virtual std::ostream& print( std::ostream& o ) const;


        // Recursive worker interfaces
        virtual std::ostream& print_recurse( std::ostream& o ) const = 0;
        virtual std::ostream& write_dot_recurse( std::ostream& o ) const = 0;
        virtual void to_mona_recurse( std::ostream& o, const std::string& prefix ) const = 0;
        virtual wali::sem_elem_t solve_recurse() = 0;


        virtual size_t hash() const;

        std::ostream& monaHeader(std::ostream& o, const std::string& prefix) const;
        std::ostream& monaName( std::ostream& o, const std::string& prefix) const;
      protected: // dot helper methods
        std::ostream& write_dot_state( std::ostream& o, const std::string& lbl, const std::string& color ) const;
        std::ostream& write_dot_edge( std::ostream& o, regex_t that ) const;

      public:
        const int id;

      protected:
        wali::sem_elem_t value;

    };

  } // namespace regex

} // namespace wali

#endif  // wali_regex_Regex_GUARD

