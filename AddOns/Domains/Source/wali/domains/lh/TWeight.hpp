#ifndef wali_domains_lh_TWEIGHT_GUARD
#define wali_domains_lh_TWEIGHT_GUARD 1

/**
 * @author Nicholas Kidd
 *
 * Wrapper around class T that implements the semiring methods of WALi. [T] must
 * be LH or PhaseLH for the moment, but TWeight will support TensorLH
 * when it is implemented.
 */

#include "wali/Common.hpp"
#include "wali/SemElem.hpp"

#include <vector>

namespace wali
{
  namespace domains
  {
    namespace lh
    {
      template<typename T> class TWeight : public wali::SemElem
      {

        public:

          TWeight( const T& x ) : SemElem() , impl(x) {}

          TWeight( const TWeight& w ) : SemElem(w) , impl(w.impl) {}

          virtual ~TWeight() {}

          T getImpl()
          {
            return impl;
          }

          ///////////////////////////////////////////////////
          // Helper instantiation methods
          static TWeight<T>* Null()
          {
            return new TWeight(T::Null());
          }

          static TWeight<T>* Empty()
          {
            return new TWeight(T::Empty());
          }

          static TWeight<T>* Id()
          {
            return new TWeight(T::Id());
          }

          static TWeight<T>* Acquire(int lock,int phase)
          {
            return new TWeight(T::Acquire(lock,phase));
          }

          static TWeight<T>* Release(int lock,int phase)
          {
            return new TWeight(T::Release(lock,phase));
          }

          static TWeight<T>* Transition(int from, int to)
          {
            return new TWeight(T::Transition(from,to));
          }

          static bool Compatible(TWeight* t1,TWeight* t2)
          {
            return T::Compatible(t1->getImpl(),t2->getImpl());
          }

          static bool Compatible( std::vector< TWeight >& v )
          {
            std::vector< T > impls;
            for( size_t i = 0; i < v.size(); i++)
            {
              impls.push_back( v[i].getImpl() );
            }
            return T::Compatible( impls );
          }

          ///////////////////////////////////////////////////
          // Semiring methods
          virtual wali::sem_elem_t zero() const
          {
            static sem_elem_t ZERO( new TWeight(T::Null()) );
            return ZERO;
            //return new TWeight( T::Null() );
          }

          virtual wali::sem_elem_t one() const
          {
            static sem_elem_t ONE( new TWeight(T::Id()) );
            return ONE;
            //return new TWeight( T::Id() );
          }

          // Turn off quasi one for all *LH weights.
          virtual wali::sem_elem_t quasi_one()
          {
            return one();
          }

          virtual wali::sem_elem_t extend( wali::SemElem * se )
          {
            TWeight* that = dynamic_cast<TWeight*>(se);
            assert(that != NULL);
            if (impl.is_null() || that->impl.is_null())
              return zero();
            /* -- This is causing trouble with PhasedLH
            else if (impl.is_id())
              return se;
            else if (that->impl.is_id())
              return this;
              */
            else
              return new TWeight( impl * that->impl );
          }

          virtual wali::sem_elem_t combine( wali::SemElem * se )
          {
            TWeight* that = dynamic_cast<TWeight*>(se);
            assert(that != NULL);
            if (impl.is_null())
              return se;
            else if (that->impl.is_null())
              return this;
            else
              return new TWeight( impl | that->impl );
          }

          virtual bool equal( wali::SemElem * se ) const
          {
            TWeight* that = dynamic_cast<TWeight*>(se);
            assert(that != NULL);
            return (impl == that->impl);
          }

          virtual std::ostream& print( std::ostream & o ) const
          {
            impl.print(o);
            return o;
          }

          virtual std::ostream& prettyPrint( std::ostream& o ) const
          {
            impl.prettyPrint(o);
            return o;
          }

          //virtual wali::sem_elem_t diff( wali::SemElem * se );

          //virtual std::pair<wali::sem_elem_t,wali::sem_elem_t> delta( wali::SemElem * se );

        protected:
          T impl;

      }; // class TWeight

    } // namespace lh

  } // namespace domains

} // namespace wali

#endif // wali_domains_lh_TWEIGHT_GUARD

