// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main() {
    int a = 1;
    int b = 3;
    for(int t = 0; t < 10; t++) {
        if (a == 0) {
            a = 1 - a;
            b --;
        } else if (a == 1) {
            a = 1 - a;
            b ++;
        } else {

        }
    }
    __VERIFIER_assert(a + b == 4);
}
