#ifndef RELATION_INTERFACE_BDD_GUARD
#define RELATION_INTERFACE_BDD_GUARD

#include "RelationInterface.h"
#include "BinRel.hpp"

namespace wali {
	namespace domains {
		namespace binrel {
/* An instance of RelationInterface for BDDs - takes a vocabulary ordering and executes the
* various operations based off of that ordering
*/

class BinRelInterface: public RelationInterface
{
	public:
		/* Functions that manipulate vocabulary placement in the BDD */
		//Construction
		//ETTODO - create a copy constructor
		BinRelInterface();
		BinRelInterface(BinRel *  n);
		BinRelInterface* Transpose();
		BinRelInterface* Detensor();
		BinRelInterface* DetensorTranspose();
		BinRelInterface* Tensor(BinRelInterface* b);
		BinRelInterface* Extend(BinRelInterface* b);
		BinRelInterface* Combine(BinRelInterface* b);
		BinRelInterface* Star();
		BinRelInterface* One();
		BinRelInterface* Zero();
		void setTensored(bool b);
		wali::domains::binrel::BinRel * n;
		/*TODO: Operations on BDDs - Extend/Combine/One/Zero/OneT/ZeroT*/
	protected:
	private:
	bool tensored;
};
}
}
}
#endif //RELATION_INTERFACE_BDD_GUARD
