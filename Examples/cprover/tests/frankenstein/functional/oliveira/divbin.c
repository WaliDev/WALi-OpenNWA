int division (int A, int B)
    {
    __VERIFIER_assume(A >=0 && B>0);
    int q,r,b;

    q=0;
    r=A;
    b=B;

    //inv(   exists   (  int   k;   k >= 0  &&  b == 2^k*B  )   &&   q == 0   &&   r == A  &&  A >= 0  &&  B > 0  );
    while(r>=b)
        {
        b=2*b;
        }

    //inv(   A == q*b + r   &&   r >= 0   &&   exists   (  int   k;   k >= 0 && b == 2^k*B  )   &&  B > 0  &&  b > r  );
    while(b!=B)
        {
        q=2*q;
        b=b/2;
        if (r>=b) 
            {
            q=q+1;
            r=r-b;
            }
        }

    assert(q==A/B);

    return r;
    }

int main () {
    int A = __VERIFIER_nondet_int();
    int B = __VERIFIER_nondet_int();
    __VERIFIER_assume(A >=0 && B>0);
    int result = division(A, B);
    __VERIFIER_assert(result == A % B);
    return 0;
}
