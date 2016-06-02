
int main() {
  int counter = 0, values = 0;
  int i;

  char input[100];
  for ( i = 0 ; i < 100 ; i ++ ) {
    input[i] = __VERIFIER_nondet_char();
  }

  for ( i = 0 ; i < 100 ; i ++ ) {
    if (input [i] == 'B') {
      counter++;
      values += 2;
    }
  }
  if ( counter == 150)
    assert(0);

  return 0;
}
