/*!
 * @author Nicholas Kidd
 * @version $Id: Union.cpp 418 2008-05-12 17:19:03Z kidd $
 */


#include "wali/regex/Union.hpp"
#include <cassert>

namespace wali 
{
  namespace regex 
  {
    Union::Union() : Regex() {
    }

    Union::~Union() {
    }

    void Union::add(regex_t r) {
      children.push_back(r);
    }

    void Union::to_mona_recurse(
        std::ostream& o, const std::string& prefix) const
    {
      if( !marked() ) {
        mark();
        std::list< regex_t >::const_iterator it = children.begin();
        for( ; it != children.end() ; it++ ) {
          regex_t c = *it;
          c->to_mona_recurse(o,prefix);
        }
        monaHeader(o,prefix);

        o << "\n    ";
        bool first = true;
        for( it = children.begin() ; it != children.end() ; it++ ) {
          if( first ) first = false;
          else o << " | ";
          regex_t c = *it;
          c->monaName(o,prefix) << "(p,q)";
        }
        o << ";\n\n";
      }
    }

    std::ostream& Union::print_recurse( std::ostream& o ) const {
      o << "(";
      std::list< regex_t >::const_iterator it = children.begin();
      bool first = true;
      for( ; it != children.end() ; it++ ) {
        if( first ) {
          first = false;
        }
        else {
          o << " | ";
        }
        regex_t r = *it;
        r->print_recurse(o);
      }
      o << ")";
      return o;
    }

    void Union::reset_marks() const
    {
      if( marked() ) {
        unmark();
        std::list< regex_t >::const_iterator it = children.begin();
        for( ; it != children.end(); it++ ) {
          regex_t child = *it;
          child->reset_marks();
        }
      }
    }

    std::ostream& Union::write_dot_recurse( std::ostream& o ) const
    {
      mark();
      std::string label("+");
      if( value.is_valid() )
        label += " . " + value->to_string();
      write_dot_state(o,label,"yellow");
      std::list< regex_t >::const_iterator it = children.begin();
      for( ; it != children.end(); it++ ) {
        regex_t child = *it;
        write_dot_edge(o,child);
        if( !child->marked() )
          child->write_dot_recurse(o);
      }
      return o;
    }

    wali::sem_elem_t Union::solve_recurse()
    {
      if( !marked() && !value.is_valid()) {
        mark();
        std::list< regex_t >::iterator it = children.begin();
        wali::sem_elem_t answer;
        for(; it != children.end(); it++ ) {
          regex_t child = *it;
          wali::sem_elem_t tmp = child->solve_recurse();
          if( answer.is_valid() ) {
            if( tmp.is_valid() ) {
              answer = answer->combine(tmp);
            }
            else {
              *wali::waliErr << "[WARNING] Concat::solve - "
                "A child was invalid (not all Root Regex's have a weight).\n";
            }
          }
          else {
            answer = tmp;
          }
        }
        value = answer;
      }
      return value;
    }

  } // namespace regex

} // namespace wali

