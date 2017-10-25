void main(int N) {
    //if (N < 0) return;
    int z, w = 1;
    for(int n = 1; n < N; n++) {
        z = 1;
        for(int m = 1; m < N; m++) {
            if (__VERIFIER_nondet_int()) { z = 2 * z; }
        }
        w = 2 * w + z;
    }
    __VERIFIER_print_hull(z);
    __VERIFIER_print_hull(w);
}
