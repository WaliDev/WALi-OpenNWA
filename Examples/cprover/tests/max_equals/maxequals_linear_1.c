// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

#define max(A,B) ( (A > B) ? A : B )

int x;

void loop() {
    for(int t = 0; t <= 10; t++) {
        x = max(x, 20 * t - 50);
    }
}

void main() {
    x = 0;
    loop();
    __VERIFIER_assert(x == 150);
}
