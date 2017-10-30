// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

#define max(A,B) ( (A > B) ? A : B )

int x,y;

void loop(int n) {
    for(int t = 0; t < 3; t++) {
        y = y + n;    // y "+=" n
        x = max(x,y); // x "max=" y
    }
}
void main() {
    x = 0;
    y = 0;
    loop(3);
    __VERIFIER_assert(y == 9); // we can prove this
    __VERIFIER_assert(x >= 9); // we can prove this
    __VERIFIER_assert(x == 9); // we can't prove this; I wish we could
}
