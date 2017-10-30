// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

int x,y;
int upper,lower;

void loop() {
    while(__VERIFIER_nondet_int()) {
        if (x + y < upper && __VERIFIER_nondet_int()) {
            if (__VERIFIER_nondet_int()) { x++; } else { y++; }
        }
        if (x + y > lower && __VERIFIER_nondet_int()) {
            if (__VERIFIER_nondet_int()) { x--; } else { y--; }
        }
    }
}

void middle() {
    x = __VERIFIER_nondet_int();
    y = x;
    loop();
    int sum = x + y;
    __VERIFIER_print_hull(sum);
    __VERIFIER_assert(sum <= upper);
    __VERIFIER_assert(sum >= lower);
}

void main() {
    upper = __VERIFIER_nondet_int();
    __VERIFIER_assume(upper >= 0);
    lower = __VERIFIER_nondet_int();
    __VERIFIER_assume(lower <= 0);
    middle();
}
