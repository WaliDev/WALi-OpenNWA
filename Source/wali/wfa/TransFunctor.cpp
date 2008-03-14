/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/WFA.hpp"

namespace wali
{
  namespace wfa
  {
    /***********
     * TransPrinter
     ***********/
    void TransPrinter::operator()( const ITrans* t )
    {
      t->print(o << "    ") << std::endl;
    }

    /***********
     * TransCopier
     ***********/
    void TransCopier::operator()( const ITrans* t )
    {
      fa.addTrans(t->from(),t->stack(),t->to(),t->weight());
    }

    /***********
     * TransDeleter
     ***********/
    void TransDeleter::operator()( ITrans* t )
    {
      { // BEGIN DEBUGGING
        //t->print( *waliErr << "\tDeleting " ) << std::endl;
      } // END DEBUGGING
      delete t;
    }

    //////////////
    // TransDotty
    //////////////
    TransDotty::TransDotty( std::ostream& theo,bool b ) :
      o(theo), print_weights(b) {}

    void TransDotty::operator()( const ITrans* t )
    {
      o << "\t";
      o << t->from();
      o << " -> ";
      o << t->to();
      o << " [color=red,label=\"";
      if( print_weights ) {
        o << "< ";
      }
      printKey(o,t->stack());
      if( print_weights ) {
        o << " | ";
        t->weight()->print( o );
        o << " >";
      }
      o << "\"];\n";
    }

    //////////////////
    // TransMarshaller
    //////////////////
    TransMarshaller::TransMarshaller( std::ostream& theo ) :
      o( theo ) {}

    void TransMarshaller::operator()( const ITrans* t )
    {
      t->marshall(o << "\t" ) << "\n";
    }

    //////////////////
    // StackHasher
    //    - used by WFA::intersect
    //////////////////
    void StackHasher::operator()( ITrans* t )
    {
      Key stack = t->stack();
      stackmap_t::iterator it = stackmap.find( stack );
      if( it == stackmap.end() )
      {
        TransSet transSet;
        it = stackmap.insert(stack,transSet).first;
      }
      it->second.insert(t);
    }

    StackHasher::iterator StackHasher::begin()
    {
      return stackmap.begin();
    }

    StackHasher::iterator StackHasher::end()
    {
      return stackmap.end();
    }

    StackHasher::iterator StackHasher::find( Key k )
    {
      return stackmap.find(k);
    }

  } // namespace wfa

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */
