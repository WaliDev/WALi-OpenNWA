#define a 0.1

void main() {
    int i, n;
    float sn=0;
    for(i=1; i<=n; i++) {
	sn = sn + a;
    }
    assert(sn==n*a || sn == 0);
}
