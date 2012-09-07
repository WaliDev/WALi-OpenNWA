/*!
 * @author Nicholas Kidd
 * @version $Id: Star.cpp 425 2008-05-14 12:00:53Z kidd $
 */

#include "wali/regex/Star.hpp"
#include <cassert>

namespace wali 
{
  namespace regex 
  {
    Star::Star( regex_t c ) : Regex(), child(c) {}

    Star::~Star() {}

    void Star::to_mona_recurse(
        std::ostream& o, const std::string& prefix) const 
    {
      if( marked() ) {
        return;
      }
      mark();
      child->to_mona_recurse(o,prefix);
      monaHeader(o,prefix);
      o << "\n    ex2 P: p in P & q in P &\n";
      o << "        all1 r,r': consecutive_in_set(r,r',P) => ";
      child->monaName(o,prefix) << "(r,r');\n\n";
      //assert(0);
    }

    std::ostream& Star::print_recurse( std::ostream& o ) const
    {
      //o << "("; child->print_recurse(o); o << ")*";
      child->print_recurse(o) << "*";
      return o;
    }

    void Star::reset_marks() const 
    {
      if( marked() ) {
        unmark();
        child->reset_marks();
      }
    }

    std::ostream& Star::write_dot_recurse(std::ostream& o ) const 
    {
      mark();
      write_dot_state(o,"*","green");
      write_dot_edge(o,child);
      if( !child->marked() )
        child->write_dot_recurse(o);
      return o;
    }

    wali::sem_elem_t Star::solve_recurse()
    {
      // Probably better to use
      // if( !value.is_valid() )
      if( !marked() && !value.is_valid()) {
        mark();
        wali::sem_elem_t w1 = child->solve_recurse();
        value = w1->one();           // start at 1
        w1 = w1->combine(w1->one()); // w1 + 1
        while (!value->equal(w1)) {  // until converge
          value = w1;                // record old value
          w1 = w1->extend(w1);       // square
        }
      }
      return value;
    }

  } // namespace regex

} // namespace wali

