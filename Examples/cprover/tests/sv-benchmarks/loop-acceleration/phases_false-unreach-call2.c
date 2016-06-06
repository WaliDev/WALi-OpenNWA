int main(void) {
  unsigned int x = 1;
  unsigned int y = __VERIFIER_nondet_uint();

  __VERIFIER_assume(y > 0);

  while (x < y) {
    if (x < y / x) {
      x *= x;
    } else {
      x++;
    }
  }

  __VERIFIER_assert(x != y);
}
