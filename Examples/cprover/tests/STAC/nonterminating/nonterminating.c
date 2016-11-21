#pragma tick

int main() {
  int i;

  for(i = 0; i < 100 || nondet(); i++) {
    tick(0);
  }
  tick(1);

  return 0;
}
