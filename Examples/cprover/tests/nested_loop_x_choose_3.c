//#include <stdio.h>

int main() {
  int x, y, i, j;

  y = 0;
  for(x = 0; x < 20; x++) {
    // Perform y = y + x*x;
    for (i = 0; i < x; i++) {
      for (j = 0; j < x; j++) {
        y++;
      }
    }
  }

  return 0;
}
