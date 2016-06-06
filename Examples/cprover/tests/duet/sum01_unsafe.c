#define a (2)

void main() { 
    int i, n=rand(), sn=0;
    assume(n >= 0);
    for(i=1; i<=n; i++) {
	if (i<10)
	    sn = sn + a;
    }
    assert(sn==n*a || sn == 0);
}
