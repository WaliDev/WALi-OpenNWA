#include "TSLWeight.hpp"

#include <iostream>
#include <sstream>

using namespace wali::domains::tsl_weight;
using std::cout;
using std::endl;
typedef wali::ref_ptr<TSLWeight> tsl_weight_t;

namespace wali
{
	namespace domains
	{
		namespace tsl_weight
		{

			static TSLWeight* convert(wali::SemElem* se)
			{
				TSLWeight* nw = dynamic_cast<TSLWeight*>(se);
				if (nw == NULL) {
					*waliErr << "[ERROR] Cannot cast to class wali::tsl_weight::TSLWeight.\n";
					se->print(*waliErr << "    ") << endl;
					assert(false);
				}
				// When done with developement
				// TSLWeight* br = static_cast<TSLWeight*>(se)
				return nw;
			}

			tsl_weight_t operator*(tsl_weight_t a, tsl_weight_t b)
			{
				return a->Compose(b);
			}

			tsl_weight_t operator|(tsl_weight_t a, tsl_weight_t b)
			{
				return a->Union(b);
			}

			tsl_weight_t operator&(tsl_weight_t a, tsl_weight_t b)
			{
				return a->Intersect(b);
			}
		}
	}
}
// ////////////////////////////
// Members and Con/Destructors
TSLWeight::TSLWeight(const TSLWeight& that) :
wali::SemElemTensor(that),
tslW(that.tslW)
{}

TSLWeight::TSLWeight(RTG::regExpRefPtr tslWeight) :
tslW(tslWeight)
{}
TSLWeight::~TSLWeight() {}
// ////////////////////////////

tsl_weight_t TSLWeight::Compose(tsl_weight_t that) const
{
	return that;
}

tsl_weight_t TSLWeight::Union(tsl_weight_t that) const
{
	return that;
}
tsl_weight_t TSLWeight::Intersect(tsl_weight_t that) const
{
	return that;
}
bool TSLWeight::Equal(tsl_weight_t that) const
{
	return (tslW == that->getWeight());
}
tsl_weight_t TSLWeight::Transpose() const
{
	return new TSLWeight(0);
}

wali::sem_elem_t TSLWeight::star()
{
	return new TSLWeight(0);
}

wali::sem_elem_t TSLWeight::combine(wali::SemElem* se)
{
	tsl_weight_t that(convert(se));
	return Union(that);
}

wali::sem_elem_t TSLWeight::extend(wali::SemElem* se)
{
	tsl_weight_t that(convert(se));
	return Compose(that);
}

bool TSLWeight::equal(wali::SemElem* se) const
{
	tsl_weight_t that(convert(se));
	return Equal(that);
}
wali::sem_elem_t TSLWeight::one() const
{
	return new TSLWeight(RTG::Zero::make());
}

wali::sem_elem_t TSLWeight::zero() const
{
	return new TSLWeight(RTG::Zero::make());
}

std::ostream& TSLWeight::print(std::ostream& o) const
{
	tslW->print(o);
	return o;
}

wali::sem_elem_tensor_t TSLWeight::transpose()
{
	return Transpose();
}

wali::sem_elem_tensor_t TSLWeight::tensor(wali::SemElemTensor* se)
{
	tsl_weight_t that(convert(se));
	return that;
}
wali::sem_elem_tensor_t TSLWeight::detensor()
{
	return new TSLWeight(RTG::Zero::make());
}

wali::sem_elem_tensor_t TSLWeight::detensorTranspose()
{
	return new TSLWeight(RTG::Zero::make());
}