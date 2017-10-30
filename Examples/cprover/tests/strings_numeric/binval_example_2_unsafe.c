int guess_binval;
int password_binval;
int result;

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

int main(int g) {
    guess_binval = g;
    password_binval = 9 * 256 * 256 * 256 +
                      8 * 256 * 256 +
                      7 * 255 + 
                      6;
    __VERIFIER_assume(guess_binval >= 0);
    __VERIFIER_assume(password_binval >= 0);
    result = checkSecret();
    __VERIFIER_assume(result == 2);

    int guess_char_0 = (guess_binval / (256 * 256 * 256)) % 256;
    int guess_char_1 = (guess_binval / (256 * 256      )) % 256;
    int guess_char_2 = (guess_binval / (256            )) % 256;

    //__VERIFIER_assert(guess_char_0 == 9);
    //__VERIFIER_assert(guess_char_1 == 8);
    __VERIFIER_assert(guess_char_2 == 7); // NOTE: SHOULD FAIL (since we assumed result==2)

    //__VERIFIER_print_hull(guess_char_0);
    //__VERIFIER_print_hull(guess_char_1);
}
