/*!
 * $Id: TransFunctor.cpp,v 1.3 2005/06/23 17:07:10 kidd Exp $
 *
 * @author Nick Kidd
 * @version $Revision: 1.3 $
 */

#include "wali/KeyFactory.hpp"
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
        void TransPrinter::operator()( const Trans* t )
        {
            t->print(o) << std::endl;
        }

        /***********
         * TransCopier
         ***********/
        void TransCopier::operator()( const Trans* t )
        {
            fa.add_trans(t->from(),t->stack(),t->to(),t->weight());
        }

        /***********
         * TransDeleter
         ***********/
        void TransDeleter::operator()( Trans* t )
        {
            {
                // TODO : REMOVE
                t->print( std::cerr << "\tDeleting " ) << std::endl;
            }
            delete t;
        }

        //////////////
        // TransDotty
        //////////////
        TransDotty::TransDotty( std::ostream& theo,bool b ) :
            o(theo), print_weights(b) {}

        void TransDotty::operator()( const Trans * t )
        {
            o << "\t";
            o << t->from_state();
            o << " -> ";
            o << t->to_state();
            o << " [color=red,label=\"";
            if( print_weights ) {
                o << "< ";
            }
            KeyFactory::print_key(o,t->stack());
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

        void TransMarshaller::operator()( const Trans * t )
        {
            t->marshall(o);
        }

    } // namespace wfa

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
