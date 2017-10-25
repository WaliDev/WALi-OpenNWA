long guess_binval;
long password_binval;
long result;

int prefixRemoving(long s, long k) { 
    for(int i = 0; i < k; i++) { 
        s /= 256; 
    } 
    return s; 
}

int checkSecret() { // What's the length of the longest correct prefix of guess?
    __VERIFIER_assume(guess_binval >= 0);
    __VERIFIER_assume(password_binval >= 0);
    // Note: This example assumes that the guess and real password are
    //   both of the same length, namely 3 characters.
    if ((password_binval / (256 * 256      )) % 256 != 
           (guess_binval / (256 * 256      )) % 256)   { return 0; }
    if ((password_binval / (256            )) % 256 != 
           (guess_binval / (256            )) % 256)   { return 1; }
    if ((password_binval                    ) % 256 != 
           (guess_binval                    ) % 256)   { return 2; }
    return 3; // all 3 characters are correct (the whole string is correct)
}

void main() {
    guess_binval = __VERIFIER_nondet_int();
    password_binval = __VERIFIER_nondet_int();
    __VERIFIER_assume(guess_binval    >= 0);
    __VERIFIER_assume(password_binval >= 0);
    __VERIFIER_assume(guess_binval    <  (256 * 256 * 256));
    __VERIFIER_assume(password_binval <  (256 * 256 * 256));

    result = checkSecret();
    __VERIFIER_print_hull(result);

    //prefixRemoving(1,1);
}
