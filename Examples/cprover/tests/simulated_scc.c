// Simulates the features of the stack manipulations used in Tarjan's
//   SCC algorithm via depth-first search.  One client of the stack 
//   (the search step) does individual pushes; the other client of 
//   the stack (emit an SCC that has been recognized) pops some 
//   number of elements off of the stack, possibly clearing the stack
//   but never going "negative".

int cost; // current (memory) usage

int hwm;  // high water mark: the largest amount of memory that we've
          //   needed at any point during this execution 

#define init_tick(k) {cost = (k); hwm = (k);}
// You could add a semicolon here, and __VERIFIER_assume(hwm >= cost)

#define tick(k) { \
                 __VERIFIER_assume(cost + (k) >= 0); \
                 __VERIFIER_assume(hwm >= cost); \
                 __VERIFIER_assume(cost >= 0); \
                 __VERIFIER_assume(hwm >= 0); \
                 cost = cost + (k); \
                 hwm = (hwm >= cost) ? hwm : cost; \
                 }

int main() {
    int N;
    int M;
    int i;
    int s; // the height of a stack variable

    init_tick(0);

    N = __VERIFIER_nondet_int();
    M = (N >= 0) ? N : 0;
    s = 0;

    for(i = 0; i < M; i++) { 
        __VERIFIER_assume(s >= 0); // FIXME we may want to remove this
        if (__VERIFIER_nondet_int() == 0) { 
            // this branch simulates initiating a new search
            s = s + 1;
            tick(1);
        } else { // this branch simulates emitting an SCC
            while(s != 0) { // FIXME Note: (s > 0) is less helpful than (s != 0)
                s = s - 1;
                tick(-1);
                //if (__VERIFIER_nondet_int() == 0) break; // FIXME uncomment this
            }
        }
    }
    //assert(s >= 0);
    //assert(cost == s);
    //assert(cost >= 0);
    while(s > 0) { // this loop simulates emitting the final SCC
        s = s - 1;
        tick(-1);
    }
    
    assert(hwm <= M); 
    assert(cost == 0); 

    return 0;
}
