//t(n) = 8t(n/2) + 1000n^2
//t(n) = 1001n^3 - 1000n^2

#include "tick.h"

void func(int n)
{ 
    if(n == 1)
    {
        tick(1);
        return;
    }
    int i;
    for (i=0; i<=7; i++){
        func(n/2);
    }
    tick(1000 * n * n);
}

int main()
{
    init_tick(0);
    
    func(8);
    assert(__cost <= 8^2);
    return 0;
}
