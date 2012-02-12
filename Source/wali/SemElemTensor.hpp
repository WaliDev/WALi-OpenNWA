#ifndef wali_SEMELEM_TENSOR_GUARD
#define wali_SEMELEM_TENSOR_GUARD 1

/**
 * @author Prathmesh Prabhu
 *
 */
#include "wali/SemElem.hpp"

namespace wali 
{
  /* forward declaration. class sem_elem_tensor_t
   * is defined first so that all of its methods
   * may be inlined by the compiler (typesafe macros)
   */
  class SemElemTensor;

  /**
   * @typedef sem_elem_tensor_t
   * Handy typedef for working with reference counted
   * SemElemTensor objects.
   */
  typedef ref_ptr<SemElemTensor> sem_elem_tensor_t;



  class SemElemTensor : public wali::SemElem 
  {
    public:
      /**
       *  @brief SemElemTensor constructor
       */ 
      //explicit SemElem( bool countme = true );
      explicit SemElemTensor();

      /**
       *  @brief SemElemTensor destructor is virtual and does nothing
       */
      virtual ~SemElemTensor() {}

      virtual sem_elem_tensor_t transpose() = 0;

      virtual sem_elem_tensor_t tensor(SemElemTensor* se) = 0;

      virtual sem_elem_tensor_t detensor() = 0;

      virtual sem_elem_tensor_t detensorTranspose() = 0;

      /**
       *  return the One element of the semiring
       */
      virtual sem_elem_t one() const = 0;

      /**
       *  return the Zero element of the semiring
       */
      virtual sem_elem_t zero() const = 0;

      /**
       *  Perform the extend operation
       */
      virtual sem_elem_t extend( SemElem * se ) = 0;

      /**
       *  Perform the combine operation
       */
      virtual sem_elem_t combine( SemElem * se ) = 0;

  };


} // namespace wali

#endif  // wali_SEMELEMTENSOR_GUARD


