// C4B output: |[0,x]|

unsigned int tick = 0;

void start(int x, int y)
{
  int z;

  while (x>y) {
	tick++;
    x-=y+1;
    for (z=y; z>0; z--)
      /* nothing */;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
  __VERIFIER_assume(y >= 0);
	start(x, y);
	
	int bnd = (x>0)?x:0;
	assert(tick <= bnd);
	
	return 0;
}
