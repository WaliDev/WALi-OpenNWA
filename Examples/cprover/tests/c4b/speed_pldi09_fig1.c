//C4B output: 2.00 |[0, n]|

unsigned int tick = 0;

void start(int n)
{
  int x=0;
  int y=0;

  for (;;) {
    if (x < n) {
      y=y+1;
      x=x+1;
      tick++;
    } else if (y > 0) {
      y=y-1;
      tick++;
	}
    else
      break;
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	
	start(n);
	
	int bnd = 2 * (n>0?n:0);
	assert (tick <= bnd);
	return 0;
}

