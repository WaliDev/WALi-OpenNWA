// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main(int N) {
    int x = 2;
    int y = 1;
    for(int t = 0; t < N; t++) {
        __VERIFIER_assert(x == y + 1 || y == x + 1);
        __VERIFIER_assume(x == y + 1 || y == x + 1);
        if (x > y) {
            y = y + 2; 
        } else { 
            x = x + 2; 
        }
    }
}
