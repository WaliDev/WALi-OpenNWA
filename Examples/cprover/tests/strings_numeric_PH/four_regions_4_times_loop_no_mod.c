int checkSecret(int _secret, int _guess)  {
    __VERIFIER_assume(_secret >= 0); __VERIFIER_assume(_secret <= 1024);
    int width = 256;
    int level = 0;
    while (level <= 3) {
        if (_secret / width != _guess / width) return level;
        width /= 4; level++;
    }
    return level;
}
int guess;
void start() {
    __VERIFIER_assume(guess >= 0); __VERIFIER_assume(guess <= 1024);
    int secret0 = __VERIFIER_nondet_int();
    int secret1 = __VERIFIER_nondet_int();
    int secret2 = __VERIFIER_nondet_int();
    int secret3 = __VERIFIER_nondet_int();
    int secret4 = __VERIFIER_nondet_int();
    int time0 = checkSecret(secret0, guess);
    int time1 = checkSecret(secret1, guess);
    int time2 = checkSecret(secret2, guess);
    int time3 = checkSecret(secret3, guess);
    int time4 = checkSecret(secret4, guess);
    __VERIFIER_assume(time1 > time0);
    __VERIFIER_assume(time2 > time1);
    __VERIFIER_assume(time3 > time2);
    __VERIFIER_assume(time4 > time3);
    __VERIFIER_print_hull(secret0);
    __VERIFIER_print_hull(secret1);
    __VERIFIER_print_hull(secret2);
    __VERIFIER_print_hull(secret3);
    __VERIFIER_print_hull(secret4);
}
int main(int argc, char ** argv) {
    guess = __VERIFIER_nondet_int();
    start();
    return 0;
}
