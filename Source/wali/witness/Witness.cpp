/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/witness/WitnessExtend.hpp"
#include "wali/witness/WitnessCombine.hpp"
#include "wali/witness/Visitor.hpp"

namespace wali
{
    namespace witness
    {
        Witness::Witness( sem_elem_t set ) : user_se(set),isEmpty(false)
        {
        }

        Witness::Witness( sem_elem_t se, bool ie) : user_se(se),isEmpty(ie)
        {
        }

        Witness::~Witness() {}

        // Test if the Witness has user weight ZERO
        bool Witness::isZero()
        {
            return user_se->equal( user_se->zero() );
        }

        // Test if the Witness has user weight ONE
        bool Witness::isOne()
        {
            return user_se->equal( user_se->one() );
        }

        sem_elem_t Witness::one() const
        {
            return new Witness( user_se->one(),true );
        }

        sem_elem_t Witness::zero() const
        {
            return new Witness( user_se->zero(),true );
        }

        sem_elem_t Witness::extend( SemElem * se )
        {
            // TODO : make this a debug check
            // and non debug use static_cast
            Witness *that = dynamic_cast< Witness * >(se);
            if( 0 == that )
            {
                *waliErr << "SemElem is \"" << typeid(se).name() << "\"\n";
                assert( 0 );
            }
            if( isEmpty && user_se->equal(user_se->one()) ) {
                return that;
            }
            else if( that->isEmpty && that->user_se->equal(that->user_se->one()) ) {
                return this;
            }
            return new WitnessExtend( user_se->extend(that->user_se), this, that );
        }

        /*
         */
        sem_elem_t Witness::combine( SemElem * se )
        {
            // TODO : make this a debug check
            // and non debug use static_cast
            Witness *that = dynamic_cast< Witness * >(se);
            if( 0 == that )
            {
                *waliErr << "SemElem is \"" << typeid(se).name() << "\"\n";
                assert( 0 );
            }

            if( isZero() ) {
                return that;
            } else if( that->isZero() ) {
                return this;
            }
            else {

                // Do this here as both branches perform the combine
                sem_elem_t combinedUserSe = user_se->combine(that->user_se);

                WitnessCombine * oldwc = dynamic_cast< WitnessCombine* >(that);
                WitnessCombine * newwc = new WitnessCombine( combinedUserSe );

                // if this or wit is a WitnessCombine then we may not
                // need to create a new WitnessCombine. There are probably
                // tradeoffs to both
                if( 0 != oldwc )
                {
                    // that is already a WitnessCombine.

                    // if A + B == A, then A <= B in the semiring.
                    // Use this fact to see if oldwc->user_se contains
                    // this->user_se. We happen to know by the way SemElem::delta
                    // works that the "old" weight on a transition will
                    // be param se (or that). So lets see if combined_se
                    // == oldwc->user_se. If so, we can ignore the weight
                    // of 'this'.
                    //
                    newwc->absorb(oldwc);
                    if( !combinedUserSe->equal( oldwc->user_se ) ) 
                    {
                        newwc->addChild(this);
                    }
                    // NO ELSE CASE b/c oldwc contains this's weight
                }
                else {
                    // Neither this nor that are of type WitnessCombine. 
                    // Simply add this and that to newwc's children.
                    newwc->addChild(that);
                    newwc->addChild(this);
                }
                return newwc;
            }
        }

        bool Witness::equal( SemElem * se ) const
        {
            // TODO : make this a debug check
            // and non debug use static_cast
            Witness *that = dynamic_cast< Witness * >(se);
            assert( 0 != that );
            return user_se->equal( that->user_se );
        }

        std::ostream& Witness::print( std::ostream& o ) const
        {
            o << "\n";
            return prettyPrint(o,0);
        }

        void Witness::accept( Visitor& v, bool visitOnce )
        {
            // TODO how does marking work...need a flag maybe
            mark();
            //assert(0);
            *waliErr << "[?ERROR?] Witness::accept called\n";
            v.visit( this );
        }

        // Adds 2 spaces and a vertical bar for each depth
        std::ostream& Witness::prettyPrint( std::ostream& o, size_t depth ) const
        {
            formatDepth( o , depth );
            o << "Witness: ";
            user_se->print(o) << std::endl;
            return o;
        }

        std::ostream& Witness::formatDepth( std::ostream& o , size_t depth ) const
        {
            for( ; depth > 0 ; depth-- )
            {
                o << "  |";
            }
            return o;
        }

    }   // namespace witness

}   // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
 */
