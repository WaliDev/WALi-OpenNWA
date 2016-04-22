// Tarjan's popall example, which he uses to illustrate the potential method.
// We've modified the example to keep track of a cost and high water mark:

int cost; // current (memory) usage

int hwm;  // high water mark: the largest amount of memory that we've
          //   needed at any point during this execution 

#define tick(k)   cost = cost + (k); hwm = (hwm >= cost) ? hwm : cost

int main() {
    int N;
    int M;
    int i;
    int s; // the height of a stack variable

    cost = 0;
    hwm = 0;

    N = __VERIFIER_nondet_int();
    M = (N >= 0) ? N : 0;
    s = 0;

    for(i = 0; i < M; i++) { // push object onto a stack
        s = s + 1;

        tick(1);

        // Note: tick(1) is equivalent to the following:

        //cost = cost + 1;
        //if (cost > hwm) { hwm = cost; }
    }

    //assert(cost <= M);   // We can prove this,
    //assert(cost <= hwm); // and this,
    assert(hwm <= M);      // but not yet this.

    while(s > 0) { // this loop performs popall()
        s = s - 1;
        tick(-1);
    }
    
    //assert(cost == 0); // We can prove this,
    //assert(hwm <= M);  // but not yet this.

    return 0;
}
