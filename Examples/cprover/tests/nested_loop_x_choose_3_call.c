//#include <stdio.h>
int y;

void p(v) {
  int i, j;

  // Perform y = y + v*v;
  for (i = 0; i < v; i++) {
    for (j = 0; j < v; j++) {
      y++;
    }
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
