int checkSecret(int _secret, int _guess, int _t)  {
    int n = 20;
    int time = 0;
    if (_guess <= _secret) {
        if (_t == 1) {
            time += 1;
        } else if (_t == 2) {
            for (int i = 0; i < n; i++) {
                time += 1;
            }
        } else {
            for (int i = 0; i < n * n * n; i++) {
                time += 1;
            }
        }
    } else {
        if (_t == 1) {
            time += 1;
        } else if (_t == 2) {
            for (int i = 0; i < n * n; i++) {
                time += 1;
            }
        } else {
            for (int i = 0; i < n * n * n; i++) {
                time += 1;
            }
        }
    }
    return time;
}

int guess; int t;

void start() {
    int secret1 = __VERIFIER_nondet_int();
    int secret2 = __VERIFIER_nondet_int();
    int time1 = checkSecret(secret1, guess, t);
    int time2 = checkSecret(secret2, guess, t);
    int timediff = time1 - time2;
    __VERIFIER_assume(timediff > 200);
    //__VERIFIER_assume(timediff <= 200);
    __VERIFIER_print_hull(secret1);
    __VERIFIER_print_hull(secret2);
    __VERIFIER_print_hull(time1);
    __VERIFIER_print_hull(time2);
}

int main(int argc, char ** argv) {
    guess = __VERIFIER_nondet_int();
    t = __VERIFIER_nondet_int();
    start();
    __VERIFIER_print_hull(t);
    __VERIFIER_print_hull(guess);
    return 0;
}
