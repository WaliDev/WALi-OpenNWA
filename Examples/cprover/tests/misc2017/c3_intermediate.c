int secret;
int checkSecret(int _secret, int _guess, int _t)  {
    int n = 20;
    int time = 0;
    if (_guess <= _secret) {
        if (_t == 1) { time += 1; }
        else if (_t == 2) { for (int i = 0; i < n; i++) { time += 1; } }
        else { for (int i = 0; i < n * n * n; i++) { time += 1; } }
    } else {
        if (_t == 1) { time += 1; }
        else if (_t == 2) { for (int i = 0; i < n * n; i++) { time += 1; } }
        else { for (int i = 0; i < n * n * n; i++) { time += 1; } }
    }
    return time;
}
#define SECRET_MAX 1000000
void attack() {
    int secretMin = 0;
    int nSecrets = SECRET_MAX;
    int steps = 0;
    __VERIFIER_assume(secret >= 0);
    __VERIFIER_assume(secret < SECRET_MAX);
    while(nSecrets >= 2) {
        __VERIFIER_assert(secret >= secretMin); // Line 24: Loop invariant part 1
        __VERIFIER_assert(secret < secretMin + nSecrets); // Line 25: Loop invariant part 2
        __VERIFIER_assume(secret >= secretMin);
        __VERIFIER_assume(secret < secretMin + nSecrets);
        nSecrets = (nSecrets + 1) / 2;
        int guess = secretMin + nSecrets;
        int time = checkSecret(secret, guess, 2);
        if (time < 210) { secretMin = guess; } else { }
        steps++;
        __VERIFIER_print_hull(steps);
    }
    __VERIFIER_assert(secret == secretMin); // Line 35: does the attack work?
    __VERIFIER_print_hull(steps); 
}
int main(int argc, char ** argv) {
    int secret = __VERIFIER_nondet_int();
    __VERIFIER_assume(secret >= 0);
    __VERIFIER_assume(secret < SECRET_MAX);
    attack();
    return 0;
}
