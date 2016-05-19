// We expect to see that x = 9 after calling main.
int x;

void p2(int v) {
  x = x + 1;
}

void p1(int v) {
  p2(v);
  p2(v);
}

int main() {
  x = 5;
  p1(3);
  p1(3);
  assert(x==9);
}
