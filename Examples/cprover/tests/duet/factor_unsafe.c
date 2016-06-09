void main() { 
    int i, n=rand();
    assume(n >= 2);
    for(i=2; i % n != 0; i++)
	;
    assert(i <= n);
}
