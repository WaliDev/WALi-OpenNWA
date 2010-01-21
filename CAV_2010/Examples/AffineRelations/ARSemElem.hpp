#ifndef AR_ARSEMELEM_GUARD
#define AR_ARSEMELEM_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "ARSemiring.hpp"
#include "wali/SemElem.hpp"
#include "wali/WeightFactory.hpp"

using wali::sem_elem_t;

namespace AR {

#define AR_SEMELEM_DIFF 0

    class ARSemElem : public wali::SemElem , public wali::WeightFactory
    {
        public:
            //---------------------
            // Constructors 
            //---------------------
            ARSemElem( ARSemiring* impl_ );
            ARSemElem(ModuleSpace *,unsigned=0);	
            virtual ~ARSemElem();

            //---------------------------------
            // semiring operations
            //---------------------------------
            virtual sem_elem_t one() const;
            virtual sem_elem_t zero() const;
            virtual sem_elem_t extend(SemElem * se);
            virtual sem_elem_t combine(SemElem * se);
            virtual bool equal(SemElem * se) const;
#if AR_SEMELEM_DIFF
            virtual sem_elem_t diff(SemElem * se) const;
#endif

            //------------------------------------
            // output
            //------------------------------------
            std::ostream & print(std::ostream &) const;

            ModuleSpace *vs();

            //------------------------------------
            // parsing
            //------------------------------------
            virtual wali::sem_elem_t getWeight( std::string& s );

            ARSemiring* getImpl() {
                return impl.get_ptr();
            }

        private:
            wali::ref_ptr<ARSemiring> impl;
    };

} // namespace AR


/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */

#endif  // AR_ARSEMELEM_GUARD
