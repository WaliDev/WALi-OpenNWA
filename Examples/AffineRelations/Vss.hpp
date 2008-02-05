#ifndef VSS_GUARD
#define VSS_GUARD 1

/*!
 * @author Nick Kidd
 */

#include "ARSemiring.hpp"
#include "wali/SemElem.hpp"
#include "wali/WeightFactory.hpp"

using wali::sem_elem_t;

class Vss : public wali::SemElem , public wali::WeightFactory
{
public:
	//---------------------
	// Constructors 
	//---------------------
    Vss( VSSemiring* impl_ );
	Vss(VectorSpace *,unsigned=0);	
    virtual ~Vss();

	//---------------------------------
	// semiring operations
	//---------------------------------
    virtual sem_elem_t one() const;
    virtual sem_elem_t zero() const;
	virtual sem_elem_t extend(SemElem * se);
    virtual sem_elem_t combine(SemElem * se);
	virtual bool equal(SemElem * se) const;
	virtual sem_elem_t diff(SemElem * se) const;

	//------------------------------------
	// output
	//------------------------------------
	std::ostream & print(std::ostream &) const;

	VectorSpace *vs();

	//------------------------------------
	// parsing
	//------------------------------------
    virtual wali::sem_elem_t getWeight( std::string& s );

    VSSemiring* getImpl() {
        return impl.get_ptr();
    }

private:
    wali::ref_ptr<VSSemiring> impl;
};

#endif  // VSS_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
*/
