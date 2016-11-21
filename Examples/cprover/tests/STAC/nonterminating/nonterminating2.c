#pragma tick

int main() {
  int i;

  for(i = 0; i != 100; i++) {
    if (nondet()) i++;   /* i == 99 could skip to 100, and then 101 before the condition is tested */
    tick(0);
  }
  tick(1);
  return 0;
}
