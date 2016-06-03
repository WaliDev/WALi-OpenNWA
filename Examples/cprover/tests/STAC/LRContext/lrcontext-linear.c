#define absVal(v)    ((v)<0 ? -(v) : (v))

int x;

void body(int y) {
  if(y <= 0) return;

  if(x >= 0) {  // With the LR-context technique, this branch will
                // not be searched: postcondition x < 0 does not hold
    x = -absVal(x);
    body(y-1);
    x = absVal(x);
  }
  else {  // With the LR-context technique, this branch will
          // not be searched: precondition x >= 0 does not hold
    x = absVal(x);
    body(y-1);
    x = -absVal(x);
  }
}

int main() {
  x = __VERIFIER_nondet_int();
  __VERIFIER_assume(x >= 0);

  body(100);    // With the LR-context technique, the search for an
                // assertion violation will be carried out with
                // a left context (precondition) of "x >= 0" and
                // a right context (postcondition) of "x < 0"

  assert(x >= 0);

  return 0;
}
