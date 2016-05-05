// C4B output: 1+3|[0,x]|+|[0,y]|

unsigned int tick = 0;

void fish() { return; }

void start(int x, int y)
{
  while (x>0) {
	tick++;
    x=x-1;
    y=y+2;
  }
  while (y>0) {
	tick++;
    y=y-1;
  }
  fish();
  while (y>0) {
	tick++;
    y=y-1;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(x, y);
	
	int bnd = 1 + 3 * ((x>0)?x:0) + ((y>0)?y:0);
	assert(tick <= bnd);
	
	return 0;
}
