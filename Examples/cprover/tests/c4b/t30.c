// C4B output: |[0,x]|+|[0,y]|

unsigned int tick = 0;

void start(int x, int y)
{
  int t;

  while (x>0) {
	tick++;
    x--;
    t=x, x=y, y=t;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(x, y);
	
	int bnd = ((x>0)?x:0) + ((y>0)?y:0);
	assert(tick <= bnd);
	
	return 0;
}
