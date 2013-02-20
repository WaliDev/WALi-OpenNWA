#ifndef WALI_WITNESS_LENGTH_WORKLIST_GUARD
#define WALI_WITNESS_LENGTH_WORKLIST_GUARD 1

/*!
 * @author Rich Joiner
 */

#include "wali/Common.hpp"
#include "wali/witness/Witness.hpp"
#include "wali/PriorityWorklist.hpp"

namespace wali
{
  namespace witness
  {
    struct ShorterThan : std::binary_function<const wfa::ITrans*, const wfa::ITrans*, bool>
    {
      bool operator()( const wfa::ITrans* a, const wfa::ITrans* b ) const
      {
        Witness *wita = dynamic_cast<Witness*>(a->weight().get_ptr());
        assert (wita && "Transition without witness used with WitnessLengthWorklist");

        Witness *witb = dynamic_cast<Witness*>(b->weight().get_ptr());
        assert (witb && "Transition without witness used with WitnessLengthWorklist");

        if (wita->getMinimumLength() > witb->getMinimumLength()) {
          return false;
        } else {
          return true;
        }

      }
    };


    class WitnessLengthWorklist : public PriorityWorklist<ShorterThan>
    {
      public:
        WitnessLengthWorklist();
        virtual ~WitnessLengthWorklist();
    }; // class WitnessLengthWorklist

  } // namespace witness

} // namespace wali

#endif  // WALI_WITNESS_LENGTH_WORKLIST_GUARD

