
/* algorithm for computing the product of two natural numbers */

int prod (int x,int y)
//pre( x >= 0 && y >= 0 ); 
    {
    __VERIFIER_assume(x >=0 && y>=0);
    int a,b,p,q;

    a = x;
    b = y;
    p = 1;
    q = 0;

   // inv( q+abp==xy );
    while( a!=0 && b!=0 ) 
        { 
        if ( a % 2 ==0 && b% 2 ==0 )
            {
            a =a/2;
            b = b/2;
            p = 4*p;
            }
        else if ( a % 2 ==1 && b% 2 ==0 )
            {
            a =a-1;
            q = q+b*p;
            }
        else if ( a % 2 ==0 && b% 2 ==1 )
            {
            b =b-1;
            q = q+a*p;
            }
        else 
            {
            a =a-1;
            b =b-1;
            q = q+(a+b-1)*p;
            }
        }
    return (q + a * b * p); 
    }
//post( result==xy );


int main () {
    int x = __VERIFIER_nondet_int();
    int y = __VERIFIER_nondet_int();
    __VERIFIER_assume(x >=0 && y>=0);
    int result = prod(x, y);
    __VERIFIER_assert(result == x * y);
    return 0;
}

