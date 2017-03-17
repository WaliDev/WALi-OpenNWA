//t(n,k) = t(n-1, k-1) + t(n-1, k) + t(n-2, k) - t(n-2, k-1)
//t(0,0) = 1
//t(n,k) = 0 if n<0 or k<0

//Property t(i,i) = 1,  t(i+1, i) = 1, and t(n,0) is Fibonacci


int func(int n, int k)
{ 
    if(n==0 && k==0)
    {
        return 1;
    }
    if (n<0 || k<0){
        return 0;
    }
   
    return func(n-1, k-1) + func(n-1, k) + func(n-2, k) - func(n-2,k-1);
}

int main()
{
    int x = __VERIFIER_nondet_int();
    __VERIFIER_assume(x>0);
    int ans = func(15, 5);
    assert(ans == 4745);
    assert(func(x, x) == 1);
    assert(func(x+1, x) == 1);
    return 0;
}
