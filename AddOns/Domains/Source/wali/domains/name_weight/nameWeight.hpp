#ifndef WALI_NAMEWEIGHT_GUARD
#define WALI_NAMEWEIGHT_GUARD 1
#include "wali/ref_ptr.hpp"
#include "wali/SemElemTensor.hpp"
namespace wali
{
  namespace domains
  {
    namespace name_weight
    {
      class NameWeight : public wali::SemElemTensor
      {
        typedef wali::ref_ptr<NameWeight> name_weight_t;
        public:
          /** @see NameWeight::Compose */
          friend name_weight_t operator*(name_weight_t a, name_weight_t b);
          /** @see NameWeight::Union */
          friend name_weight_t operator|(name_weight_t a, name_weight_t b);
          /** @see NameWeight::Intersect */
          friend name_weight_t operator&(name_weight_t a, name_weight_t b);
        public:
          NameWeight(const NameWeight& that);
          NameWeight(int name1, int name2);
          virtual ~NameWeight();
        public:
          name_weight_t Compose( name_weight_t that ) const;
          name_weight_t Union( name_weight_t that ) const;
          name_weight_t Intersect( name_weight_t that ) const;
          bool Equal(name_weight_t se) const;
          name_weight_t Transpose() const;
           public:
          // ////////////////////////////////
          // SemElem methods
          sem_elem_t one() const;
          sem_elem_t zero() const;

          bool isOne() const {
            return false;
          }

          bool isZero() const {
            return false;
          }

          /** @return [this]->Union( cast<NameWeight*>(se) ) */
          sem_elem_t combine(SemElem* se);

          /** @return [this]->Compose( cast<NameWeight*>(se) ) */
          sem_elem_t extend(SemElem* se);

          sem_elem_t star();


          /** @return [this]->Equal( cast<NameWeight*>(se) ) */
          bool equal(SemElem* se) const;

          std::ostream& print( std::ostream& o ) const;
          // ////////////////////////////////
          // SemElemTensor methods

          /** @return [this]->Transpose() */
          sem_elem_tensor_t transpose();

          sem_elem_tensor_t tensor(SemElemTensor* se);

          sem_elem_tensor_t detensor();

          sem_elem_tensor_t detensorTranspose();

          int getName1() const {
            return nm1;
          }

          int getName2() const{
            return nm1;
          }

          // ////////////////////////////////
          // Printing functions
          //static void printHandler(FILE *o, int var);
        protected:
          int nm1;
          int nm2;
      };
    }
  }
}
#endif
