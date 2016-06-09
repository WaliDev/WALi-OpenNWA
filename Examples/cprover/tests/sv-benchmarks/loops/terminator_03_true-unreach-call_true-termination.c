#define LIMIT 1000000

int main() {
    int x=__VERIFIER_nondet_int();
    int y=__VERIFIER_nondet_int();
    __VERIFIER_assume(y <= LIMIT);

    if (y>0) {
        while(x<100) {
            x=x+y;
        }
    }

    __VERIFIER_assert(y<=0 || (y>0 && x>=100));

    return 0;
}


