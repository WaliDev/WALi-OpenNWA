// C4B output: |[y,x]|

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
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(x, y);
	
	int bnd = (x>y)?(x-y):0;
	assert(tick <= bnd);
	
	return 0;
}
