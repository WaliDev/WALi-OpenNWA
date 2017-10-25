int guess_binval;
int password_binval;
int result;

int checkSecret() {
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

int main(int g, int p) {
    guess_binval = g;
    password_binval = p;
    result = checkSecret();
}
