int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int n)
{
  int x=0;

  while (x<n) {
    x=x+1;
    while (x<n) {
	  tick++;
      if (__VERIFIER_nondet_int())
        break;
      x=x+1;
    }
  }
}

int main() 
{
	start(20);
	return 0;
}

