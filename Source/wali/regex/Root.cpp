/*!
 * @author Nicholas Kidd
 * @version $Id: Root.cpp 418 2008-05-12 17:19:03Z kidd $
 */

#include "wali/regex/Root.hpp"

namespace wali 
{
  namespace regex 
  {
    static Key getNilKey() {
      static Key k = getKey("$");
      return k;
    }

    static Key getIdKey() {
      static Key k = getKey("");
      return k;
    }

    regex_t Root::make(Key k,sem_elem_t se)
    {
      if (k == getNilKey())
        return NIL();
      else if (k == getIdKey())
        return ID();
      else
        return new Root(k,se);

    }

    regex_t Root::ID()
    {
      static regex_t R( new Root(getIdKey()) );
      return R;
    }

    regex_t Root::NIL()
    {
      static regex_t R( new Root(getNilKey()) );
      return R;
    }

    Root::~Root() {
    }

    // Override Regex 
    bool Root::isConstant() const 
    { 
      return true; 
    }

    std::ostream& Root::print_recurse( std::ostream& o ) const {
      printKey(o,lbl);
      return o;
    }

    void Root::to_mona_recurse( 
        std::ostream& o, const std::string& prefix ) const 
    {
      if( !marked() ) {
        mark();
        o << "pred " << prefix << "_" << id << "(var1 p, var1 q) = is_" << key2str(lbl) << "(p,q);\n\n";
      }
    }

    std::ostream& Root::write_dot_recurse( std::ostream& o ) const
    {
      write_dot_state(o,key2str(lbl),"blue");
      return o;
    }

    wali::sem_elem_t Root::solve_recurse()
    {
      return value;
    }

    Root::Root(Key label) : Regex(),lbl(label)
    {
    }

    Root::Root(Key label, wali::sem_elem_t se) : Regex(se),lbl(label)
    {
    }

  } // namespace regex

} // namespace wali

