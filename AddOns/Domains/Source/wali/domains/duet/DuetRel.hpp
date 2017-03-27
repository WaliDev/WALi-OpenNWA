#ifndef wali_duetrel_DUETREL_GUARD
#define wali_duetrel_DUETREL_GUARD 1

#include <map>
#include <vector>
#include <utility>
#include <string>
#include <vector>

extern "C" {
#include <caml/mlvalues.h>
}

#include <boost/shared_ptr.hpp> // It'd be nice to include the standard version but there are too many ways to get it.
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>

#include "wali/wfa/WeightMaker.hpp"
#include "wali/Countable.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/SemElemTensor.hpp"
#include "wali/Key.hpp"

#include "../nparel/NPARel.hpp"

// FIXME Allow weights to be collected
#define MAX_WEIGHT_COUNT 10000000

using namespace wali::domains::nparel;

namespace wali 
{
  namespace domains 
  {
    namespace duetrel 
    {
      class DuetRel;
      typedef wali::ref_ptr<DuetRel> duetrel_t;

      //class DuetRelPair;
      //typedef wali::ref_ptr<DuetRelPair> duetrelpair_t;
      //class DuetRelPair : public Countable {
      //  public:
      //    duetrel_t first;
      //    duetrel_t second;

      //    DuetRelPair(duetrel_t v1, duetrel_t v2) : Countable(), 
      //                                              first(v1), 
      //                                              second(v2) {}

      //    ~DuetRelPair() {}

      //    static duetrelpair_t MkDuetRelPair(duetrel_t v1, duetrel_t v2);
      //};
      
      class DuetRel : public NPARel
      {
        public:
          static void reset();

          ///** @see DuetRel::Compose */
          //friend duetrel_t operator*(duetrel_t a, duetrel_t b);
          ///** @see DuetRel::Union */
          //friend duetrel_t operator|(duetrel_t a, duetrel_t b);
          ///** @see DuetRel::Intersect */
          //friend duetrel_t operator&(duetrel_t a, duetrel_t b);
          
          static duetrel_t MkDuetRel(value w, bool is_tensored=false);

          // ////////////////////////////////
          // DuetRel methods
          static duetrel_t getBaseOne();
          static duetrel_t getBaseZero();
          static duetrel_t getBaseTop();
          static duetrel_t getTensorOne();
          static duetrel_t getTensorZero();
          duetrel_t Compose( duetrel_t that ) const;
          duetrel_t Union( duetrel_t that ) const;
          //duetrel_t Intersect( duetrel_t that ) const;
          //duetrel_t Normalize() const;
          bool Equal(duetrel_t se) const;
          bool IsSat() const;
          duetrel_t Transpose() const;
          duetrel_t Kronecker( duetrel_t that) const;
          nparel_t Merge(int c, int v) const;
          //duetrel_t TensorMerge(int c, int v) const;
          
          //duetrelpair_t alphaHatStar(duetrel_t previousAbstractValue = 0);
          nparelpair_t alphaHatStar(nparel_t previousAbstractValue = 0);
          //bool Equivalent(duetrel_t that) const;
          bool Equivalent(nparel_t that) const;

          virtual size_t hash() const {
            return static_cast<size_t>(getValue());
            //return 0;
          }

          bool isWTensored()
          {
            return isTensored;
          }

          void setTensored(bool tens)
          {
            isTensored = tens;
          }

          // ////////////////////////////////
          // Printing functions
          std::ostream& print( std::ostream& o ) const;
          std::ostream& printIndented( std::ostream& o, unsigned int indent ) const;
          std::ostream& printHull( std::ostream& o, unsigned int indent, int var ) const;
          std::ostream& printAbstract( std::ostream& o ) const;
          std::ostream& printSmtlib( std::ostream& o ) const;

          // ////////////////////////////////
          // SemElem methods
          sem_elem_t one() const;
          sem_elem_t zero() const;
          sem_elem_t tensorOne() const;
          sem_elem_t tensorZero() const;

          bool isOne() const;

          bool isZero() const;
        
          /** @return [this]->Union( cast<DuetRel*>(se) ) */
          sem_elem_t combine(SemElem* se);

          /** @return [this]->Compose( cast<DuetRel*>(se) ) */
          sem_elem_t extend(SemElem* se);

          sem_elem_t star();

          /** @return [this]->Equal( cast<DuetRel*>(se) ) */
          bool equal(SemElem* se) const;

          // ////////////////////////////////
          // SemElemTensor methods

          /** @return [this]->Transpose() */
          sem_elem_tensor_t transpose();

          /** @return [this]->Kronecker( cast<DuetRel*>(se) ) */
          sem_elem_tensor_t tensor(SemElemTensor* se);

          sem_elem_tensor_t detensor();

          sem_elem_tensor_t detensorTranspose();

          /** @return The backing BDD */
          value getValue() const;

          virtual bool containerLessThan(SemElem const * other) const;

          //static void printHandler(FILE *o, int var);

          int relId;

        public: 
          static bool simplify;
          static bool simplifyOnPrint;
          static bool printOnAlphaHatStar;

        protected:
          bool isTensored;
          static int wCnt;
          static value caml_weights[MAX_WEIGHT_COUNT];

        public:
          virtual ~DuetRel();

        private:
          DuetRel(const DuetRel& that);
          DuetRel(int d, bool is_tensored=false);
      };

    } // namespace duetrel

  } // namespace domains

} // namespace wali

#endif  // wali_duet_DuetRel_GUARD

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


