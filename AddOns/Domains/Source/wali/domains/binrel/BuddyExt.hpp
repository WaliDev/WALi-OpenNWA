#include "buddy/fdd.h"

inline bdd biimp(int maxVal, int idx1, int idx2)
{
  bdd id = bddfalse;
  for(int ctr = 0; ctr < maxVal; ++ctr)
    id = id |
      (fdd_ithvar(idx1,ctr) &
       fdd_ithvar(idx2,ctr));
  return id;
}
