/* Division algorithm from "Z. Manna, Mathematical Theory of Computation, McGraw-Hill, 1974" */

#include "assert.h"

void main () {
    int x1, x2;
    __VERIFIER_assume(x1 >= 0);
    __VERIFIER_assume(x2 > 0);
    int y1,y2,y3;
    
    y1 = 0;
    y2 = 0;
    y3 = x1;
    
    while(y3 != 0) {
	if (y2 + 1 == x2) {

	    y1 = y1 + 1;
	    y2 = 0;
	    y3 = y3 - 1;
	}

	else {
	    y2 = y2 + 1;
	    y3 = y3 - 1;
	}
    }
    __VERIFIER_assert(y1 * x2 + y2 == x1);
}
