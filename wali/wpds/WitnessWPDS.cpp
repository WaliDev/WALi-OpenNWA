/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/Worklist.hpp"
#include "wali/wfa/WFA.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wpds/WitnessWPDS.hpp"
#include "wali/wpds/LinkedTrans.hpp"
#include "wali/witness/WitnessTrans.hpp"
#include "wali/witness/WitnessRule.hpp"

//
// TODO : move this elsewhere
//
namespace wali
{
    using wfa::WFA;
    using wfa::Trans;
    using witness::WitnessTrans;

    namespace wpds
    {
        /*!
         * @class WitnessTransCopyLinker
         *
         * Along with creating LinkedTrans objects for the local WFA used in
         * pre/poststar, this class overrides TransCopyLinker's operator()()
         * so that the weight passed to the LinkedTrans constructor is a
         * WitnessTrans.
         *
         * TODO: subclass TransCopyLinker
         */
        class WitnessTransCopyLinker : public wali::wfa::ConstTransFunctor
        {
            public:
                WitnessTransCopyLinker( WitnessWPDS & w, WFA & faout, Worklist * worklist );
                virtual ~WitnessTransCopyLinker();
                virtual void operator()( const ::wali::wfa::Trans * t );

                WitnessWPDS & wpds;
                WFA & fa;
                Worklist * worklist;
        };
    }

}

namespace wali
{
    using witness::WitnessRule;
    using witness::WitnessTrans;

    namespace wpds
    {
        // @brief Constructor passes wl to WPDS
        WitnessWPDS::WitnessWPDS( Worklist * wl ) : WPDS(wl)
        {
        }

        // @brief Destructor does nothing
        WitnessWPDS::~WitnessWPDS()
        {
        }

        // @brief create rule with two r.h.s. stack symbols
        // After rule creation the weight is replaced with a WitnessRule
        bool WitnessWPDS::add_rule(
                wali_key_t from_state,
                wali_key_t from_stack,
                wali_key_t to_state,
                wali_key_t to_stack1,
                wali_key_t to_stack2,
                sem_elem_t se )
        {
            rule_t r;
            bool rb = WPDS::add_rule(
                    from_state,from_stack,to_state,to_stack1,to_stack2,se,r );
            r->weight( new WitnessRule(r) );
            return rb;
        }

        //
        // link input WFA transitions to Configs
        // overrides WPDS::copy_and_link so that a WitnessTranses
        // are being created
        //
        void WitnessWPDS::copy_and_link( const WFA & in, WFA & dest )
        {
            WitnessTransCopyLinker linker( *this,dest,worklist );
            in.for_each( linker );
        }

        /////////////////////////////////////////////////////////////////
        // WitnessTransCopyLinker
        /////////////////////////////////////////////////////////////////
        WitnessTransCopyLinker::WitnessTransCopyLinker( WitnessWPDS & w, WFA & faout, Worklist * wl )
            : wpds(w),fa(faout),worklist(wl) {}

        WitnessTransCopyLinker::~WitnessTransCopyLinker() {}

        void WitnessTransCopyLinker::operator()( const Trans * orig )
        {
            Config *c = wpds.make_config( orig->from(),orig->stack() );

            // Passes a WitnessTrans as the weight to the LinkedTrans
            LinkedTrans *t =
                new LinkedTrans(orig->from(),orig->stack(),orig->to(),
                        new WitnessTrans(*orig), c);
                        //orig->weight(),c);

            // fa.add_trans takes ownership of passed in pointer
            fa.add_trans( t );

            // add t to the worklist for saturation
            worklist->put( t );
        }

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
