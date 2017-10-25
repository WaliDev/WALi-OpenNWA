int func(int n, int high) {
    int x = 1;
    int y = 2;
    int time = 0;
    int temp = 0;
    if (high) { 
        while(n > 0) { temp = x; x = -y; y = temp; time = time + x + 3; n--; }
    } else { 
        while(n > 0) { temp = x; x = -y; y = temp; time = time - y + 4; n--; }
    }
    return time;
}

void main(int argc, char ** argv) {
    int n1, n2, time1, time2, high1, high2;
    n1 = __VERIFIER_nondet_int();
    n2 = __VERIFIER_nondet_int();
    high1 = __VERIFIER_nondet_int();
    high2 = __VERIFIER_nondet_int();
    __VERIFIER_assume(n1 >= 0);
    __VERIFIER_assume(n1 == n2);
    time1 = func(n1, high1);
    time2 = func(n2, high2);
    assert(time1 - time2 <= n1);
    return 0;
}

