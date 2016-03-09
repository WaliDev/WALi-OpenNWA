// We expect to see that x = 0 after calling main.
int x;

void p() {
  x = x - 1;
}

int main() {
  x = 5;
  while (x > 0) {
  	p();
  }
}
