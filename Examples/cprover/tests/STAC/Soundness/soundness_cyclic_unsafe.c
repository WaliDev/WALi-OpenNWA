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

void p4() {
  p3();
  p3();
}


int main() {
  int k;

  for(k = 0; k < 4; k++) {
    p4();
  }

  assert(1 == 0);  /* Assert that the point is unreachable */
                   /* However, this point is reachable, with x == 64 */

  return 0;
}
