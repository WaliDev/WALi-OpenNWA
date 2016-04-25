int cost; // current (memory) usage

int hwm;  // high water mark: the largest amount of memory that we've
          //   needed at any point during this execution 

#define init_tick(k) {cost = (k); hwm = (k);}
// You could add a semicolon here, and __VERIFIER_assume(hwm >= cost)

#define tick(k) { \
                 __VERIFIER_assume(k <= cost); \
                 __VERIFIER_assume(hwm >= cost); \
                 __VERIFIER_assume(cost >= 0); \
                 __VERIFIER_assume(hwm >= 0); \
                 cost = cost + (k); \
                 hwm = (hwm >= cost) ? hwm : cost; \
                 }

