//#include <stdio.h>

int main() {
  int x, y, j;

  y = 0;
  for(x = 0; x < 20; x++) {
    // Perform y = y + x;
    for (j = 0; j < x; j++) {
      y++;
    }
  }

  return 0;
}
