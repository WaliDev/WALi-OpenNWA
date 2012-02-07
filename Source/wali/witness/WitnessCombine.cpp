/*!
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/Visitor.hpp"
#include "wali/witness/VisitorDot.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <typeinfo>
#include <iterator>

namespace wali
{
  namespace witness
  {
    //
    // Pass weight to base class Witness
    //
    WitnessCombine::WitnessCombine( sem_elem_t the_weight ) :
      Witness( the_weight ) {}

    //
    // Destructor does nothing.
    //
    WitnessCombine::~WitnessCombine() {}

    //
    // Override Witness::combine
    //
    sem_elem_t WitnessCombine::combine( SemElem * se )
    {
      // TODO : make this a debug check
      // and non debug use static_cast
      Witness *that = dynamic_cast< Witness * >(se);
      if( 0 == that )
      {
        // error checking
        *waliErr << "SemElem is \"" << typeid(se).name() << "\"\n";
        assert( 0 );
      }

      if( isZero() ) {
        return that;
      }
      else if( that->isZero() ) {
        return this;
      }
      else {
        // Perform the combine here b/c both branches need the combined
        // weight.
        sem_elem_t combined_se;
        try {
          combined_se = user_se->combine(that->weight());
        } catch (int error_code ) {
          (void) error_code;
          std::ofstream thisStream("combine.this.dot");
          std::ofstream thatStream("combine.that.dot");
          VisitorDot d1(thisStream);
          accept(d1,true);
          VisitorDot d2(thatStream);
          that->accept(d2,true);
          thisStream.close();
          thatStream.close();
          assert(0);
        }

        if( combined_se->equal( this->user_se ) ) {
          return this;
        }
        else if( combined_se->equal( that->weight() ) ) {
          return that;
        }
        else {
          // Always create a new WitnessCombine b/c cycles cause mucho problemo
          // with ref counting.
          WitnessCombine * newwc = new WitnessCombine( combined_se );

          // TODO - NAK fix for case of common subchildren
          // I.e., this = +{A,B,C}
          //       that = +{C,D,E}
          //       return +{A,B,C,D,E}
          //
          //    currently returns +{this,that} which has two copies of C
          //
          //WitnessCombine * oldwc = dynamic_cast< WitnessCombine* >(that);
          //if( 0 != oldwc ) {
          //}
          //else {
          //}

          newwc->addChild(this);
          newwc->addChild(that);
          return newwc;
        }
      }
    }

    //
    // Override Witness::accept
    //
    void WitnessCombine::accept( Visitor& v, bool visitOnce )
    {
      if( !marked() || !visitOnce ) {
        if( v.visitCombine(this) ) {
          std::list< witness_t >::iterator it = children().begin();
          for( ; it != children().end(); it++ ) {
            witness_t& child = *it;
            child->accept(v,visitOnce);
          }

          v.postvisitCombine(this);
        }
      }
    }

    //
    // Override Witness::reset_marks
    //
    void WitnessCombine::reset_marks() const
    {
      if( marked() ) {
        std::list< witness_t >::const_iterator it = kids.begin();
        for( ; it != kids.end() ; it++ ) {
          (*it)->reset_marks();
        }
      }
    }

    //
    // Override Witness::prettyPrint
    //
    std::ostream& WitnessCombine::prettyPrint( std::ostream& o, size_t depth ) const
    {
      formatDepth(o,depth);
      o << "WitnessCombine: ";
      user_se->print(o);
      // Dumps out to much information.
      // TODO. Make a compile or runtime flag.
#if 1
      o << std::endl;
      std::list< witness_t >::const_iterator it = kids.begin();
      std::list< witness_t >::const_iterator itEND = kids.end();
      for( ; it != itEND ; it++ )
      {
        (*it)->prettyPrint(o,depth+1);
      }
#else
      o << "   has " << kids.size() << " children\n";
#endif
      return o;

    }

    //
    // Add a child to this
    //
    void WitnessCombine::addChild( witness_t w )
    {
      //*waliErr << "[WitnessCombine::addChild]\n";
      kids.push_back( w );
    }

    //
    // The combine of all the weights of the children should (must)
    // equal the weight of this WitnessCombine object.
    //
    // @return reference to list of Witness children.
    //
    std::list< witness_t >& WitnessCombine::children()
    {
      return kids;
    }

    void WitnessCombine::absorb( WitnessCombine * wc )
    {
      std::copy(wc->children().begin(),wc->children().end(),std::back_inserter(kids));
    }

  } // namespace witness

} // namespace wali

