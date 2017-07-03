//t(n) = t(sqrt(n)) + 1, t(2) = 1
//t(n) = log_2(log_2(n)) + 1

#include <math.h>
#include "tick.h"

void func(int n)
{ 
    tick(1);
    if(n == 2)
    {
        return;
    }
    func(sqrt(n));
}

int main()
{
    init_tick(0);

        
    func(65536); //5
    //can't verify without log eval and sqrt(n)
    assert(__cost >15);
    return 0;
}
