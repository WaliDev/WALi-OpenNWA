//#include <stdio.h>
int y;

void p(v) {
  int j;

  // Perform y = y + v;
  for (j = 0; j < v; j++) {
    y++;
  }
}

int main() {
  int x;

  y = 0;
  for(x = 0; x < 20; x++) {
    p(x);
  }

  return 0;
}
