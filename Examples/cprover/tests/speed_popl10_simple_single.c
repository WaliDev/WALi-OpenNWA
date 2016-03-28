unsigned int tick = 0;

unsigned int __VERIFIER_nondet_uint();

void start(int n)
{
  int x=0;

  while (x<n) {
    if (__VERIFIER_nondet_uint())
      x=x+1;
    else 
      x=x+1;
    tick++;
  }
}

int main() {
  start(20);

  return 0;
}
