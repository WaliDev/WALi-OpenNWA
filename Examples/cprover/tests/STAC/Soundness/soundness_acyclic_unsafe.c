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

  assert(1 == 0);  /* Assert that the point is unreachable */
                   /* However, this point is reachable, with x == 8 */

  return 0;
}
