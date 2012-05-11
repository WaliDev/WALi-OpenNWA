/*!
 * @author Nicholas Kidd
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
      //fa.addTrans(t->from(),t->stack(),t->to(),t->weight());
      ITrans *tc = t->copy();
      fa.insert(tc);
    }

    /***********
     * TransDuplicator
     ***********/
    void TransDuplicator::operator()( const ITrans* t )
    {
      ITrans *tc;
      std::map< Key, Key >::iterator fit, tit, eit;
      fit = dup.find(t->from());
      tit = dup.find(t->to());
      eit = dup.end();

      if(fit == eit && tit == eit) {
        tc = t->copy();
      } else if(fit == eit && tit != eit) {
        tc = t->copy(t->from(), t->stack(), dup[t->to()]);
      } else if(fit != eit && tit == eit) {
        tc = t->copy();
        fa.insert(tc);

        tc = t->copy(dup[t->from()], t->stack(), t->to());
      } else {
        tc = t->copy(t->from(), t->stack(), dup[t->to()]);
        fa.insert(tc);

        tc = t->copy(dup[t->from()], t->stack(), dup[t->to()]);
      }

      fa.insert(tc);
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


    /***********
     * TransZeroWeight
     ***********/
    void TransZeroWeight::operator()( ITrans* t )
    {
      sem_elem_t wt = t->weight();
      if(wt->equal(wt->zero())) {
        zeroWeightTrans.insert(t);
      }
    }


    //////////////
    // TransDotty
    //////////////
    TransDotty::TransDotty( std::ostream& theo,bool b, DotAttributePrinter * attribute_printer ) :
      o(theo), print_weights(b), printer(attribute_printer) {}

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
      o << "\"";
      if (printer) {
        printer->print_extra_attributes(t, o);
      }
      o << "];\n";
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

