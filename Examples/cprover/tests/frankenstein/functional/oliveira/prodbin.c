
/* algorithm for computing the product of two natural numbers */

int prod (int a,int b)
//pre( a >= 0 && b >= 0 ); 
    {
    __VERIFIER_assume(a >=0 && b>=0);
    int x,y,z;

    x = a;
    y = b;
    z = 0;

    //inv( z+x*y==a*b );
    while( y!=0 ) 
        { 
        if ( y % 2 ==1 )
            {
            z = z+x;
            y = y-1;
            }
        x = 2*x;
        y = y/2;
        }
    return z; 
    }
//post( result==a*b );


int main () {
    int a = __VERIFIER_nondet_int();
    int b = __VERIFIER_nondet_int();
    __VERIFIER_assume(a >=0 && b>=0);
    int result = prod(a, b);
    __VERIFIER_assert(result == a * b);
    return 0;
}

