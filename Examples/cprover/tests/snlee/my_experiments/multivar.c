extern int __VERIFIER_nondet_int();

int main() {
    //does not work with nondet. Kincaid does not use them much himself, though.
    int n = 12;//__VERIFIER_nondet_int();
    int m = 17;

    int i, j;
    i = j = 0;
    
    int r = 0;

    for(; i < n; i++) {
        int r_old = r;

	for(j = 0; j < m; j++) {
	    r++;
	}

        assert(r == r_old + m); 
    }


    assert(r == n*m);

    return 0;
}    
