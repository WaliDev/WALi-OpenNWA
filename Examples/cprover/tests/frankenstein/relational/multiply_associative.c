int multiply(int n, int m) {
    __VERIFIER_assume(n >= 0);
    int result = 0;
    for(int i = 0; i < n; i++) {
	result += m;
    }
    return result;
}

void main() {
    int x = __VERIFIER_nondet_int();
    int y = __VERIFIER_nondet_int();
    int z = __VERIFIER_nondet_int();
    __VERIFIER_assume(x >= 0);
    __VERIFIER_assume(y >= 0);
    __VERIFIER_assume(z >= 0);

    __VERIFIER_assert(multiply(x,multiply(y,z)) == multiply(multiply(x,y),z));
}
