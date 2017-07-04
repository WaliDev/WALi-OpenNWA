int pow2(int n) {
    int i;
    int p = 1;
    __VERIFIER_assume(n > 0);
    for(i = 0; i < n; i++) {
        p *= 2;
    }
    __VERIFIER_print_hull(p);
    return p;
}

int log2(int n) {
    int i;
    int l = 0;
    // We can't handle this if we replace the conditional with i != n.
    __VERIFIER_assume(n > 0);
    for(i = 1; i != n; i *= 2) {
        l ++;
    }
    __VERIFIER_print_hull(l);
    return l;
}

void main() {
    int n = __VERIFIER_nondet_int();
    __VERIFIER_assert(n == log2(pow2(n)));
}
