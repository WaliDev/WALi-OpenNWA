
/* algorithm searching for a divisor for factorization, by Knuth */


int factor (int n, int dd)
//pre(    n % 2 == 1    &&    n > 0    &&    dd % 2 == 1    &&    ( dd - 1 ) ^3 < 8*n    );
    {
    __VERIFIER_assume(n % 2 == 1    &&    n > 0    &&    dd % 2 == 1    &&    ( dd - 1 ) ^3 < 8*n);
    int r,rp,q,d,s,t;

    d=dd;
    r=n % d;
    rp=n % (d-2);
    q=4*(n/(d-2) - n/d);
    s = __VERIFIER_nondet_int();
    __VERIFIER_assume(s * s <= n   &&   ( s + 1 ) * ( s + 1 ) > n);

    //inv(    d^2*q - 4*r*d + 4*rp*d - 2*q*d + 8*r == 8*n    &&    d % 2 == 1   );
    while((s>=d)&&(r!=0))
        {
        if (2*r-rp+q<0)
            {
            t=r;
            r=2*r-rp+q+d+2;
            rp=t;
            q=q+4;
            d=d+2;
            } 
        else if ((2*r-rp+q>=0)&&(2*r-rp+q<d+2))
            {
            t=r;
            r=2*r-rp+q;
            rp=t;
            d=d+2;
            }
        else if ((2*r-rp+q>=0)&&(2*r-rp+q>=d+2)&&(2*r-rp+q<2*d+4))
            {
            t=r;
            r=2*r-rp+q-d-2;
            rp=t;
            q=q-4;
            d=d+2;
            }
        else /* ((2*r-rp+q>=0)&&(2*r-rp+q>=2*d+4)) */
            {
            t=r;
            r=2*r-rp+q-2*d-4;
            rp=t;
            q=q-8;
            d=d+2;
            }
        }

    __VERIFIER_assert(  ( r != 0) || ( n % d == 0 )   );

    return d;
    }
//post( true ); 

int main () {
    int n = __VERIFIER_nondet_int();
    int dd = __VERIFIER_nondet_int();
    __VERIFIER_assume(n % 2 == 1    &&    n > 0    &&    dd % 2 == 1    &&    ( dd - 1 ) ^3 < 8*n);
    int result = factor(n, dd);
    return 0;
}

