//t(n) = t(n-1)^2, t(0) = 2
//t(n) = 2^2^n

#include "tick.h"

int func(int n) {
    if (n==0) {
        tick(2);
        return 2;
    }
    int x = func(n-1) * func(n-1);
    tick(x);
    return (x);
}

int main() {
    init_tick(0);
    
    func(4);
    //can't verify since can't evaluate exp
    assert(__cost == 6000);
    return 0;
}
