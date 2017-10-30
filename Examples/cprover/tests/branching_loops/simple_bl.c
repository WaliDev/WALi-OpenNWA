// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main() {
    int d = 1;
    int N = 10;
    for(int t = 0; t < N; t++) {
        if (d == 1 || d == -1) {
            d = - d;
        } else {
            d = 5;
        }
    }
    __VERIFIER_assert(d == 1 || d == -1);
    __VERIFIER_print_hull(d);
}
