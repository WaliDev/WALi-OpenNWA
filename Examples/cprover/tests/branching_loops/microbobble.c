// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main() {
    int a = 1;
    int b = 3;
    for(int t = 0; t < 10; t++) {
        a = 1 - a;
        if (a == 1) {
            b --;
        } else if (a == 0) {
            b ++;
        } else {

        }
    }
    __VERIFIER_assert(a + b == 4);
}
