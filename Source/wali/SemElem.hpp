#ifndef wali_SEM_ELEM_GUARD
#define wali_SEM_ELEM_GUARD 1

/**
 * @author Nicholas Kidd
 */

#include "wali/Common.hpp"
#include "wali/ref_ptr.hpp"
#include "wali/Countable.hpp"
#include "wali/Printable.hpp"
#include <sstream>
#include <cstdlib>
#include <string>
#include <typeinfo>

/* forward declaration (for SWIG wrapper)
 */
struct _object;
typedef struct _object PyObject;

namespace wali
{
  /* forward declaration. class sem_elem_t
   * is defined first so that all of its methods
   * may be inlined by the compiler (typesafe macros)
   */
  class SemElem;

  /**
   * @typedef sem_elem_t
   * Handy typedef for working with reference counted
   * SemElem objects.
   */
  typedef ref_ptr<SemElem> sem_elem_t;

  /** 
   * @class SemElem
   * @brief base abstract class to be subclassed by user's semiring
   *
   * This abstract class provides the signature (or in Java interface) for a
   * Semiring Element (or Weight).  All pure abstract methods must be
   * defined by the user. 
   *
   * The constructor takes a boolean that turns reference counting on or
   * off. By default it is on. Please leave this as true less you know
   * you do not want the user weight objects to be garbage collected.
   * @see ref_ptr
   *
   * Some minimal testing is provided by
   *
   *   o   wali::test_semelem_impl(wali::sem_elem_t x)
   *
   * It performs tests such as the following:
   *   o   0 = 0
   *   o   1 = 1
   *   o   x = x
   *   o   x + 0 = x = 0 + x
   *   o   x * 0 = 0 = 0 * x
   *   o   x * 1 = x = 1 * x
   */

  class SemElem : public Printable, public Countable
  {

    public:
      static const std::string XMLTag;

    public:

      /**
       *  @brief SemElem constructor
       */ 
      //explicit SemElem( bool countme = true );
      explicit SemElem();

      /**
       *  @brief SemElem destructor is virtual and does nothing
       */
      virtual ~SemElem() {}

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

      /**
       *  Equality comparison between two semiring elements
       */
      virtual bool equal( SemElem * se ) const = 0;

      /**
       *  Determines whether 'this' underapproximates 'that'
       *
       *  The following should be equivalent:
       *      this.underApproximates(that)
       *      this + that = that
       *  (Where = is ->equals and + is ->combine.)
       *
       *  The latter is what the default implementation performs
       */
      virtual bool underApproximates(SemElem * that);

      /**
       *  Print the semiring element to the std::ostream o
       */
      virtual std::ostream& print( std::ostream & o ) const = 0;

      /**
       *  Used to print weights to XML. Default implementation
       *  simply calls this->toString() and sends the output
       *  to the passed in ostream o.
       */
      virtual std::ostream& marshall( std::ostream& o ) const;

      /**
       *  Used to print weights to XML. Outer wrapper that 
       *  places the appropriate tags and then calls the virtual
       *  method marshall.
       */
      std::ostream& marshallWeight( std::ostream& o ) const;

      /**
       *  Perfrom the diff operation
       *   NOTE: This method performs (this - se).  This is very
       *         important as diff is not commutative
       */
      virtual sem_elem_t diff( SemElem * se );

      /**
       *  Perform the quasi_one operation.
       *  Quasi one by default returns the One semiring element.
       *  If you are unsure of how to use Quasione then leave the
       *  default implementation.
       */
      virtual sem_elem_t quasi_one() const;

      /**
       *  Perform delta operation
       *   The delta operation is defined for the user but can be
       *   overridden to get better performance. The std::pair returned
       *   must have two properties
       *       1) the first element == this COMBINE se
       *       2) the second element == this DIFF se
       * 
       *  @param se is the weight to be combined/diffed with "this"
       *  
       *  @return std::pair<sem_elem_t,sem_elem_t> the delta
       */
      virtual std::pair<sem_elem_t,sem_elem_t> delta( SemElem * se );

      /**
       *  Perform the star operation (returns this^\inf)
       */
      virtual sem_elem_t star();

      /** 
       * Wrapper method for extend that will remove the ref_ptr
       * to make the call to the user's code. 
       */
      sem_elem_t extend( sem_elem_t se ) 
      { 
        return extend( se.get_ptr() ); 
      }

      /** 
       * Wrapper method for combine that will remove the ref_ptr
       * to make the call to the user's code. 
       */
      sem_elem_t combine( sem_elem_t se ) 
      { 
        return combine( se.get_ptr() ); 
      }

      /** 
       * Wrapper method for equal that will remove the ref_ptr
       * to make the call to the user's code. 
       */
      bool equal( sem_elem_t se ) const 
      { 
        return equal( se.get_ptr() ); 
      }

      bool underApproximates( sem_elem_t se )
      { 
        return underApproximates( se.get_ptr() ); 
      }
      
      /**
       * Wrapper method for diff that will remove the ref_ptr
       * to make the call to the user's code. 
       */
      sem_elem_t diff( sem_elem_t se ) 
      { 
        return diff( se.get_ptr() ); 
      }

      /** 
       * Wrapper method for delta that will remove the ref_ptr
       * to make the call to the user's code. This is just a 
       * nice "typesafe macro" b/c sem_elem_t is used by WPDS
       */
      std::pair< sem_elem_t,sem_elem_t > delta( sem_elem_t se )
      {
        return delta( se.get_ptr() );
      }


      virtual
      sem_elem_t
      extendAndDiff(sem_elem_t next, sem_elem_t subtrahend);


#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4716) // must return a value
#endif                           // (std::abort not annotated as noreturn)
      /// Returns true if this is less than the other. Should be
      /// fast. HOWEVER -- does not need to have semantic meaning, other than
      /// it must be consistent with equal() and different objects
      /// representing the same semiring element should return consistent
      /// results.
      ///
      /// Default implementation aborts.
      virtual
      bool
      containerLessThan(SemElem const * other) const
      {
        (void) other;
        std::abort();
        //TODO: Have this as a pure virtual function.
        return false;
      }

      
      bool
      containerLessThan(sem_elem_t se) const
      {
        return this->containerLessThan(se.get_ptr());
      }

      virtual
      size_t
      hash() const
      {
        std::abort();
        //TODO:Have this as a pure virtual function.
        return false;
      }
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

      virtual
      std::ostream &
      print_typename(std::ostream & os) const;

      std::string
      toStringTypename() const {
          std::stringstream ss;
          print_typename(ss);
          return ss.str();
      }

      virtual
      PyObject* getuserdata() {
          return NULL;
      }
  };

  /**
   * Simple test method that can be used when developing
   * a SemElem implementation. Tests include
   *
   *  o 0 = 0
   *  o 1 = 1
   *  o x = x
   *  o x + 0 = x = 0 + x
   *  o x * 0 = 0 = 0 * x
   *  o x * 1 = x = 1 * x
   */
  void test_semelem_impl(sem_elem_t x);



  struct SemElemRefPtrContainerLessThan
  {
    bool
    operator() (sem_elem_t left, sem_elem_t right) const
    {
      return left->containerLessThan(right);
    }
  };

  struct SemElemRefPtrHash
  {
    size_t
    operator() (sem_elem_t se) const
    {
      return se->hash();
    }
  };

  struct SemElemRefPtrEqual
  {
    bool
    operator() (sem_elem_t left, sem_elem_t right) const
    {
      return left->equal(right);
    }
  };
}
#endif  // wali_SEM_ELEM_GUARD

