int guess, needToGuessAgain;

void getUsersGuess() {
  guess = __VERIFIER_nondet_int();
  if (30 <= guess && guess <= 150) {
      needToGuessAgain = 0;
  } else {
      needToGuessAgain = 1;
  }
}

void main() {
  needToGuessAgain = 1;
  while(needToGuessAgain != 0) {
      getUsersGuess();
  }
  __VERIFIER_print_hull(guess);
  __VERIFIER_assert(30 <= guess && guess <= 150);
}
