long guess_binval;
long password_binval;
long result;

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
    password_binval = (9 * 256 * 256) +
                      (8 * 256) +
                      (7);
    __VERIFIER_assume(guess_binval    >= 0);
    __VERIFIER_assume(password_binval >= 0);
    __VERIFIER_assume(guess_binval    <  (256 * 256 * 256));
    __VERIFIER_assume(password_binval <  (256 * 256 * 256));

    result = checkSecret();
    __VERIFIER_assume(result == 2);

    int guess_prefix_1char = guess_binval / (256 * 256);
    __VERIFIER_print_hull(guess_prefix_1char);
    //__VERIFIER_assert(guess_prefix_1char == 9); // Should PASS (and does)

    int guess_prefix_2char = guess_binval / (256);
    __VERIFIER_print_hull(guess_prefix_2char);
    //__VERIFIER_assert(guess_prefix_2char == 9 * 256 + 8); // Should PASS (and does)

    int guess_prefix_3char = guess_binval;
    __VERIFIER_print_hull(guess_prefix_3char);
    __VERIFIER_assert(guess_prefix_3char == 9 * 256 * 256 + 8 * 256 + 7); // Should FAIL (and does)
    //__VERIFIER_assert(guess_prefix_3char != 9 * 256 * 256 + 8 * 256 + 7); // Should PASS (and does)

    return 0;
}
