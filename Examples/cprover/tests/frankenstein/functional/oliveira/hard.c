void division(int N, int D)
//pre( N >= 0 && D > 0 );
    { 
    __VERIFIER_assume(N >= 0 && D > 0);
    int r,ds,p,q;

    r=N;
    ds=D;
    p=1;
    q=0;

    //inv( N >= 0 && D > 0 && q == 0 && r == N && ds == D*p && exists( int k; k>=0 && ds==D*2^k) && exists( int l; l>=0 && p==2^l ) );
    while ( r>= ds )
        {
        ds=2*ds;
        p=2*p;
        }

     //inv( D > 0 && N == q*D+r && ds > r && r >= 0 && ds == D*p && exists( int k; k>=0 && ds==D*2^k) && exists( int l; l>=0 && p==2^l ) );
     while ( p!=1 )
         {
         ds=ds/2;
         p=p/2;

         if ( r>=ds ) 
             {
             r=r-ds;
             q=q+p;
             }
         }
    __VERIFIER_assert( N == q*D+r && r >= 0 && r < D );
    }
//post( N == q*D+r && r >= 0 && r < D );

int main (){
    int N = __VERIFIER_nondet_int();
    int D = __VERIFIER_nondet_int();
    __VERIFIER_assume(N >= 0 && D > 0);
    int result = division(N, D);
    //__VERIFIER_assert( N == q*D+r && r >= 0 && r < D );
    return 0;
}

