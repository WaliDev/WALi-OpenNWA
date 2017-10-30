// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

int x,y;

void loop() {
    x = y;
    while(__VERIFIER_nondet_int()) {
        if (x + y < 5 && __VERIFIER_nondet_int()) {
            if (__VERIFIER_nondet_int()) { x++; } else { y++; }
        }
        if (x + y > -3 && __VERIFIER_nondet_int()) {
            if (__VERIFIER_nondet_int()) { x--; } else { y--; }
        }
    }
}

void main() {
    x = __VERIFIER_nondet_int();
    y = x;
    loop();
    int sum = x + y;
    __VERIFIER_print_hull(sum);
    __VERIFIER_assert(sum <= 5);
    __VERIFIER_assert(sum >= -3);
}
