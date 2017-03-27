#ifndef wali_nparel_DUETREL_GUARD
#define wali_nparel_DUETREL_GUARD 1

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

namespace wali 
{
  namespace domains 
  {
    namespace nparel 
    {
      class NPARel;
      typedef wali::ref_ptr<NPARel> nparel_t;

      class NPARelPair;
      typedef wali::ref_ptr<NPARelPair> nparelpair_t;
      class NPARelPair : public Countable {
        public:
          nparel_t first;
          nparel_t second;

          NPARelPair(nparel_t v1, nparel_t v2) : Countable(), 
                                                    first(v1), 
                                                    second(v2) {}

          ~NPARelPair() {}

          static nparelpair_t MkNPARelPair(nparel_t v1, nparel_t v2);
      };
      
      class NPARel : public wali::SemElemTensor
      {
        public:
          explicit NPARel();

          //static nparel_t MkNPARel(value w, bool is_tensored=false);

          // ////////////////////////////////
          // NPARel methods
          
          // Not needed:
          //virtual nparel_t Compose( nparel_t that ) const; // this actually means extend
          //virtual nparel_t Union( nparel_t that ) const;
          //virtual nparel_t Intersect( nparel_t that ) const;
          //virtual nparel_t Normalize() const;
          //virtual bool Equal(nparel_t se) const;
          //virtual nparel_t Transpose() const;
          //virtual nparel_t Kronecker( nparel_t that) const;

          //bool isWTensored()
          //{
          //  return isTensored;
          //}

          //void setTensored(bool tens)
          //{
          //  isTensored = tens;
          //}
          
          //virtual nparel_t TensorMerge(int c, int v) const; // Not currently used

          //virtual bool isOne() const;

          //virtual bool isZero() const;
        
          // ATTENTION, IMPLEMENTORS:
          //   You probably want to implement these get methods statically inside your NPARel
          //   subclass NewtonCompare.cpp will refer to them with something like 
          //      Relation::getBaseOne() 
          //   that is, using the "Relation" typedef defined in NewtonCompare.hpp
          //static nparel_t getBaseOne();
          //static nparel_t getBaseZero();
          //static nparel_t getBaseTop();
          //static nparel_t getTensorOne();
          //static nparel_t getTensorZero();

          virtual bool IsSat() const = 0; // for assertion-checking

          virtual nparel_t Merge(int c, int v) const = 0; // project away the locals of a procedure
          
          virtual nparelpair_t alphaHatStar(nparel_t previousAbstractValue = 0) = 0; // star, with an optional widening parameter

          // The following function tests equivalence of two iteration-domain elements.
          //   It should not be confused with equal(), which tests equivalence of
          //   two weights, rather than iteration-domain elements.
          virtual bool Equivalent(nparel_t that) const = 0;

          virtual size_t hash() const = 0;

          // ////////////////////////////////
          // Printing functions
          virtual std::ostream& print( std::ostream& o ) const = 0;
          virtual std::ostream& printIndented( std::ostream& o, unsigned int indent ) const = 0;

          virtual std::ostream& printAbstract( std::ostream& o ) const = 0; // print an iteration-domain element

          virtual std::ostream& printHull( std::ostream& o, unsigned int indent, int var ) const = 0; // for resource-usage bounds generation

          virtual std::ostream& printSmtlib( std::ostream& o ) const = 0; // for writing out procedure summaries in SMTLIB format

          // ////////////////////////////////
          // SemElem methods
          virtual sem_elem_t one() const = 0;
          virtual sem_elem_t zero() const = 0;
          virtual sem_elem_t tensorOne() const = 0;
          virtual sem_elem_t tensorZero() const = 0;

          /** @return [this]->Union( cast<NPARel*>(se) ) */
          virtual sem_elem_t combine(SemElem* se) = 0;

          /** @return [this]->Compose( cast<NPARel*>(se) ) */
          virtual sem_elem_t extend(SemElem* se) = 0;

          virtual sem_elem_t star() = 0; // we use alphaHatStar in preference to this, now

          // Do not confuse this function with the Equivalent() function above
          /** @return [this]->Equal( cast<NPARel*>(se) ) */
          virtual bool equal(SemElem* se) const = 0;

          // ////////////////////////////////
          // SemElemTensor methods

          /** @return [this]->Transpose() */
          virtual sem_elem_tensor_t transpose() = 0;

          /** @return [this]->Kronecker( cast<NPARel*>(se) ) */
          virtual sem_elem_tensor_t tensor(SemElemTensor* se) = 0;

          virtual sem_elem_tensor_t detensor() = 0; // probably not used

          virtual sem_elem_tensor_t detensorTranspose() = 0;

          virtual bool containerLessThan(SemElem const * other) const = 0;

          //static void printHandler(FILE *o, int var);

        protected:
          //bool isTensored;

        public:
          virtual ~NPARel();

        protected:
          NPARel(const NPARel& that);
      };

    } // namespace nparel

  } // namespace domains

} // namespace wali

#endif  // wali_nparel_NPARel_GUARD

// Yo, Emacs!
// Local Variables:
//   c-file-style: "ellemtel"
//   c-basic-offset: 2
// End:


