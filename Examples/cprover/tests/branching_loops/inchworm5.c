// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main(int N) {
    int a = 1, x = 1, y = 1;
    for(int n = 0; n < N; n++) {
        a = 1 - a;
        if (a == 1) { 
            x = y + 1;
            a = a - 1;
        } else {
            y = x;
            a = 1;
        }
    }
    __VERIFIER_assert(x - y + a == 1);
}
