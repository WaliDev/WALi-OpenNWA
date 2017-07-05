#include "assert.h"

/* Source: Timos Antonopoulos, Paul Gazzillo, Michael Hicks, Eric Koskinen,
   Tachio Terauchi, Shiyi Wei: Decomposition Instead of Self-Composition for
   Proving the Absence of Timing Channels.  PLDI 2017 */
int loginSafe(char *password, int password_len, char* guess, int guess_len) {
    int matches = 1;
    int tick = 0;
    
    for (int i = 0; i < guess_len; i++) {
	tick++;
	if (i < password_len) {
	    if (password[i] != guess[i]) {
		matches = 0;
	    }
	}
    }
    // return matches;
    return tick;
}

void main() {
    int password_len1 = __VERIFIER_nondet_int();;
    int password_len2 = __VERIFIER_nondet_int();;
    int guess_len = __VERIFIER_nondet_int();;
    __VERIFIER_assume(password_len1 >= 0);
    __VERIFIER_assume(password_len2 >= 0);
    __VERIFIER_assume(guess_len >= 0);
    
    char* password1 = malloc(password_len1);
    char* password2 = malloc(password_len2);
    char* guess = malloc(guess_len);

    __VERIFIER_assert(loginSafe(password1,password_len1,guess,guess_len)
		      == loginSafe(password2,password_len2,guess,guess_len));
}
