// We expect to see that x = 0 after calling main.
int x;

void p() {
  if (x == 0) return;
  x = x - 1;
  p();
}

int main() {
  x = 5;
  p();
}
