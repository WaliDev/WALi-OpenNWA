int __VERIFIER_nondet_int();
unsigned int tick = 0;


void start(int x, int y)
{
  while (x>y) {
	tick++;
    if (__VERIFIER_nondet_int())
      y=y+1;
    else
      x=x-1;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
