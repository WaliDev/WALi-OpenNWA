int checkSecret(int _secret, int _guess)  {
    __VERIFIER_assume(_secret >= 0); __VERIFIER_assume(_secret <= 1024);
    int width = 256;
    if (_secret / width == _guess / width) { return 1; }
    return 0;
}
int guess;
void start() {
    __VERIFIER_assume(guess >= 0); __VERIFIER_assume(guess <= 1024);
    int secret0 = __VERIFIER_nondet_int();
    int secret1 = __VERIFIER_nondet_int();
    int time0 = checkSecret(secret0, guess);
    int time1 = checkSecret(secret1, guess);
    __VERIFIER_assume(time1 > time0);
    __VERIFIER_print_hull(secret0);
    __VERIFIER_print_hull(secret1);
}
int main(int argc, char ** argv) {
    guess = __VERIFIER_nondet_int();
    start();
    return 0;
}
