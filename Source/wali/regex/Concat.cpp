/*!
 * @author Nicholas Kidd
 */

#include <cassert>
#include "wali/regex/Concat.hpp"

namespace wali
{
  namespace regex 
  {
    Concat::Concat( regex_t l, regex_t r)
      : Regex(), left(l), right(r) {}

    Concat::~Concat() {}

    void Concat::to_mona_recurse(std::ostream& o, const std::string& prefix) const
    {
      if( !marked() ) {
        mark();
        left->to_mona_recurse(o,prefix);
        right->to_mona_recurse(o,prefix);
        monaHeader(o,prefix);
        o << "\n    ex1 r where p <= r & r <= q: ";
        left->monaName(o,prefix) << "(p,r) & ";
        right->monaName(o,prefix) << "(r,q);\n\n";
      }
    }

    std::ostream& Concat::print_recurse( std::ostream& o ) const {
      o << "(";
      left->print_recurse(o);
      right->print_recurse(o);
      o << ")";
      return o;
    }

    void Concat::reset_marks() const {
      if( marked() ) {
        unmark();
        left->reset_marks();
        right->reset_marks();
      }
    }

    std::ostream& Concat::write_dot_recurse( std::ostream& o ) const
    {
      mark();
      std::string label("^");
      if( value.is_valid() )
        label = label + " . " + value->to_string();
      write_dot_state(o,label,"red");
      write_dot_edge(o,left);
      write_dot_edge(o,right);
      if( !left->marked() )
        left->write_dot_recurse(o);
      if( !right->marked() )
        right->write_dot_recurse(o);
      return o;
    }

    wali::sem_elem_t Concat::solve_recurse()
    {
      if( !marked() && !value.is_valid()) {
        mark();
        wali::sem_elem_t lse = left->solve_recurse();
        wali::sem_elem_t rse = right->solve_recurse();
        if( lse.is_valid() && rse.is_valid() )
          value = lse->extend(rse);
        else {
          *wali::waliErr << "[WARNING] Concat::solve - "
            "A child was invalid (not all Root Regex's have a weight).\n";
          value = 0;
        }
      }
      return value;
    }

  } // namespace regex

} // namespace wali

/*!
 * @version $Id: Concat.cpp 418 2008-05-12 17:19:03Z kidd $
 */

