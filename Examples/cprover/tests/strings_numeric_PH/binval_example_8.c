long guess_binval;
long password_binval;

long password_binval1;
long result1;
long password_binval2;
long result2;
long password_binval3;
long result3;
long password_binval4;
long result4;

int checkSecret() { // What's the length of the longest correct prefix of guess?
    __VERIFIER_assume(guess_binval >= 0);
    __VERIFIER_assume(password_binval >= 0);
    // Note: This example assumes that the guess and real password are
    //   both of the same length, namely 3 characters.
    if ((password_binval / (256 * 256)) % 256 != 
           (guess_binval / (256 * 256)) % 256)   { return 0; }
    if ((password_binval / (256      )) % 256 != 
           (guess_binval / (256      )) % 256)   { return 1; }
    if ((password_binval              ) % 256 != 
           (guess_binval              ) % 256)   { return 2; }
    return 3; // all 3 characters are correct (the whole string is correct)
}

int main() {
    guess_binval = __VERIFIER_nondet_int();
    __VERIFIER_assume(guess_binval    >= 0);
    __VERIFIER_assume(guess_binval    <  (256 * 256 * 256));

    password_binval1 = __VERIFIER_nondet_int();
    password_binval2 = __VERIFIER_nondet_int();
    password_binval3 = __VERIFIER_nondet_int();
    password_binval4 = __VERIFIER_nondet_int();
    __VERIFIER_assume(password_binval1 >= 0);
    __VERIFIER_assume(password_binval1 <  (256 * 256 * 256));
    __VERIFIER_assume(password_binval2 >= 0);
    __VERIFIER_assume(password_binval2 <  (256 * 256 * 256));
    __VERIFIER_assume(password_binval3 >= 0);
    __VERIFIER_assume(password_binval3 <  (256 * 256 * 256));
    __VERIFIER_assume(password_binval4 >= 0);
    __VERIFIER_assume(password_binval4 <  (256 * 256 * 256));

    password_binval = password_binval1;
    result1 = checkSecret();
    password_binval = password_binval2;
    result2 = checkSecret();
    password_binval = password_binval3;
    result3 = checkSecret();
    password_binval = password_binval4;
    result4 = checkSecret();

    __VERIFIER_assume(result1 < result2);
    __VERIFIER_assume(result2 < result3);
    __VERIFIER_assume(result3 < result4);

    __VERIFIER_print_hull(guess_binval);

    return 0;
}
