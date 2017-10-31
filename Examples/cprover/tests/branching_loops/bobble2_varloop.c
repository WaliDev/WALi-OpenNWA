// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main(int N) {
    int a = 1; int b = 3; int c = 5;
    for(int t = 0; t < N; t++) {
        if (a > 0) {
            b += a; c += 1; a = -1;
        } else {
            if (a < 0) {
                b += a; c -= 1; a = 1;
            } else {
                a = a; b = b; c = c;
            }
        }
    }
    __VERIFIER_assert(c - b == 2);
}
