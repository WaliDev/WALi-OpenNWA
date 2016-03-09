// We expect to see that x = 0 after calling main.
int x;

void p() {
  while(x > 0) {
    x = x - 1;
  }
}

int main() {
  x = 5;
  p();
}
