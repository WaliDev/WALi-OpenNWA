//t(n+1) = t(n) + 2n^2 + 3n + 4, t(0) = 1
//t(n) = 1/6(4n^3 + 3n^2 + 17n + 6)

#include "tick.h"

void func(int n)
{ 
    if(n == 0)
    {
        tick(1);
        return;
    }
    func(n-1);
    //int i;
    //int b = 2*(n-1)*(n-1) + 3*(n-1)+4;
    //for (i=0; i<b;i++){
    //for (i=0; i<2*(n-1)*(n-1) + 3*(n-1)+4;i++){
    //  tick(1);
    //}
    tick(2*(n-1)*(n-1) + 3*(n-1) + 4);
}

int main()
{
    init_tick(0);
    
    func(15);
     __VERIFIER_print_hull(__cost);
    assert(__cost == 2400);
    return 0;
}
