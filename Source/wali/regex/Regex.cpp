/*!
 * @author Nicholas Kidd
 */

#include "wali/hm_hash.hpp"
#include "wali/regex/Regex.hpp"
#include "wali/regex/Root.hpp"
#include "wali/regex/Concat.hpp"
#include "wali/regex/Union.hpp"
#include "wali/regex/Star.hpp"

namespace wali 
{
  namespace regex 
  {
    const std::string Regex::lblID("");
    const std::string Regex::lblNIL("$");


    regex_t Regex::ID()
    {
      return Root::ID();
    }

    regex_t Regex::NIL()
    {
      return Root::NIL();
    }

    regex_t Regex::COMBINE( regex_t lhs, regex_t rhs ) 
    {
      if( lhs.get_ptr() == rhs.get_ptr() )
        return lhs;
      else if( lhs->isZero() )
        return rhs;
      else if( rhs->isZero() )
        return lhs;
      else {
        Union* un = new Union();
        un->add(lhs);
        un->add(rhs);
        return un;
      }
    }

    regex_t Regex::EXTEND( regex_t lhs, regex_t rhs )
    {
      if( lhs->isZero() || rhs->isZero() )
        return NIL();
      else if( lhs->isOne() )
        return rhs;
      else if( rhs->isOne() )
        return lhs;
      else
        return new Concat(lhs,rhs);
    }

    regex_t Regex::STAR( regex_t r )
    {
      if( r->isZero() )
        return ID();
      else if( r->isOne() ) // should be fine
        return r;
      else
        return new Star(r);
    }

    static int next_alloc() {
      static int cnt = 0;
      int tmp = cnt;
      cnt++;
      return tmp;
    }

    Regex::Regex() 
      : wali::Markable(), wali::SemElem(), id(next_alloc()), value(0) {
      }

    Regex::Regex(wali::sem_elem_t se) 
      : wali::Markable(), wali::SemElem(), id(next_alloc()), value(se) {
      }

    Regex::~Regex() {
    }

    size_t Regex::hash() const
    {
      wali::hm_hash<int> hasher;
      return hasher(id);
    }

    std::ostream& Regex::print( std::ostream& o ) const 
    {
      print_recurse(o);
      // Probably not necessary
      reset_marks();
      return o;
    }

    std::ostream& Regex::monaHeader(std::ostream& o, const std::string& prefix) const
    {
      monaName(o << "pred ",prefix) << "(var1 p, var1 q) = ";
      return o;
    }

    std::ostream& Regex::monaName(std::ostream& o, const std::string& prefix) const
    {
      o << prefix << "_" << id;
      return o;
    }

    bool Regex::isOne() const 
    {
      Root* oneRoot = (Root*)one().get_ptr();
      if( this == oneRoot )
        return true;
      else {
        const Root* root = dynamic_cast<const Root*>(this);
        return (0 != root) && (root->lbl == oneRoot->lbl);
      }
    }

    bool Regex::isZero() const 
    {
      Root* zeroRoot = (Root*)zero().get_ptr();
      if( this == zeroRoot )
        return true;
      else {
        const Root* root = dynamic_cast<const Root*>(this);
        return (0 != root) && (root->lbl == zeroRoot->lbl);
      }
    }

    bool Regex::isConstant() const 
    {
      return false;
    }

    wali::sem_elem_t Regex::one() const
    {
      static wali::sem_elem_t ONE( ID().get_ptr() );
      return ONE;
    }

    wali::sem_elem_t Regex::zero() const
    {
      static wali::sem_elem_t ZERO( NIL().get_ptr() );
      return ZERO;
    }

    bool Regex::equal( wali::SemElem * se ) const
    {
      Regex* re = dynamic_cast<Regex*>(se);
      assert(re);
      if( isZero() && re->isZero() )
        return true;
      else if( isOne() && re->isOne() )
        return true;
      else
        return false; // always perform the combine
    }

    wali::sem_elem_t Regex::combine( wali::SemElem* se )
    {

      Regex* re = dynamic_cast<Regex*>(se);
      assert(re);
      regex_t res = COMBINE(this,re);
      wali::sem_elem_t ans(res.get_ptr());
      return ans;
    }

    wali::sem_elem_t Regex::extend( wali::SemElem* se )
    {
      Regex* re = dynamic_cast<Regex*>(se);
      assert(re != 0);
      regex_t r = EXTEND(this,re);
      wali::sem_elem_t ans(r.get_ptr());
      return ans;
    }

    void Regex::reset_marks() const
    {
      unmark();
    }

    // TODO: make const
    void Regex::toMona( std::ostream& o, const std::string & prefix ) const
    {
      to_mona_recurse(o,prefix);
      reset_marks();
    }

    wali::sem_elem_t Regex::solve()
    {
      wali::sem_elem_t answer = solve_recurse();
      reset_marks();
      return answer;
    }

    std::ostream& Regex::write_dot( std::ostream& o ) const
    {
      o << "digraph \"Regex_" << id << "\" {\n";
      write_dot_recurse( o );
      o << "}\n";
      reset_marks();
      return o;
    }

    std::ostream& Regex::write_dot_state( std::ostream& o, 
        const std::string& lbl, const std::string& color ) const 
    {
      o << "\t" << id <<" [label=\"" << lbl << "\",color=" << color << "];\n";
      return o;
    }

    std::ostream& Regex::write_dot_edge( std::ostream& o, regex_t that ) const {
      o << "\t" << id << " -> " << that->id << ";\n";
      return o;
    }

  } // namespace regex

} // namespace wali

/*!
 * @version $Id: Regex.cpp 418 2008-05-12 17:19:03Z kidd $
 */
