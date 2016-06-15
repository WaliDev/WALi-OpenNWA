int x = 0;

void r0() {
  x = x+1;
}

void r1() {
  r0();
  r0();
}

void r2() {
  r1();
  r1();
}

void r3() {
  r2();
  r2();
}

void r4() {
  r3();
  r3();
}


void q0() {
  int k;

  for(k = 0; k < 4; k++) {
    r4();
  }
}

void q1() {
  q0();
  q0();
}

void q2() {
  q1();
  q1();
}

void q3() {
  q2();
  q2();
}

void q4() {
  q3();
  q3();
}


void p0() {
  int k;

  for(k = 0; k < 4; k++) {
    q4();
  }
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

  p4();

  assert(1 == 0);  /* Assert that the point is unreachable */
                   /* However, this point is reachable, with x == 65536 */

  return 0;
}
