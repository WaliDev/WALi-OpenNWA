int main() {
  int x, y;
  x = y = 0;

  for (int i = 0; i < x; i++) {
    y = y + 2;
  }
  
  assert(y == 2 * y);
}
