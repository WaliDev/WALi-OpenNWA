int x = 0;

void p0() {
  x = x+1;
}

void p1() {
  p0();
  p0();
}

void p2() {
  p1();
  p1();
}

void p3() {
  p2();
  p2();
}

int main() {
  p3();

  assert(x == 8);

  return 0;
}
