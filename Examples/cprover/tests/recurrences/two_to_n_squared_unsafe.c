//t(n) = 2^(2n-1)*t(n-1), t(0) = 1
//t(n) = 2^n^2

#include <math.h>
#include "tick.h"

void func(int n)
{
    if(n == 0)
    {
        tick(1);
        return;
    }
    int i;
    for (i=0; i<pow(2, 2*n-1); i++){
        func(n-1);
    }
}

int main()
{
    init_tick(0);
    
    func(4);
    //can't verify due to evaluating exp
    assert(__cost == 6000);
    return 0;
}
