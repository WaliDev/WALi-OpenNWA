long guess_binval;
long password_binval;
long result;

int checkSecret() {
    __VERIFIER_assume(guess_binval >= 0);
    __VERIFIER_assume(password_binval >= 0);
    // Note: This example assumes that the guess and real password are
    //   both of the same length, namely 4 characters.
    if ((password_binval / (256 * 256 * 256)) % 256 != 
           (guess_binval / (256 * 256 * 256)) % 256) {
        return 0;
    }
    if ((password_binval / (256 * 256      )) % 256 != 
           (guess_binval / (256 * 256      )) % 256) {
        return 1;
    }
    if ((password_binval / (256            )) % 256 != 
           (guess_binval / (256            )) % 256) {
        return 2;
    }
    if ((password_binval                    ) % 256 != 
           (guess_binval                    ) % 256) {
        return 3;
    }
    return 4;
}

int main() {
    guess_binval = __VERIFIER_nondet_int();
    password_binval = (9 * 256 * 256 * 256) +
                      (8 * 256 * 256) +
                      (7 * 255) + 
                      (6);
    __VERIFIER_assume(guess_binval    >= 0);
    __VERIFIER_assume(password_binval >= 0);

    // NOTE: the L suffixes on the following line are needed so that
    //   CIL does not get confused:
    __VERIFIER_assume(guess_binval    <  (256L * 256L * 256L * 256L));

    // WHOA! big problem with large literal values

    //__VERIFIER_assume(password_binval <  (256 * 256 * 256 * 256));
    result = checkSecret();
    __VERIFIER_assert(0); // Should FAIL
    //__VERIFIER_assume(result == 2);
    //__VERIFIER_assert(result != 0);
    //__VERIFIER_print_hull(result);

    return 0;
}
