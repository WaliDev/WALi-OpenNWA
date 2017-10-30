// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

#define max(A,B) ( (A > B) ? A : B )

int x;
int n;

void loop() {
    for(int t = 0; t <= 10; t++) {
        x = max(x, n * t + 50);
    }
}

void main(int initial) {
    x = initial;
    n = (__VERIFIER_nondet_int()) ? 10 : -10;
    loop();
    __VERIFIER_print_hull(x);
    __VERIFIER_assert(x >= initial);
}
