// C4B output: |[x,y]|+|[y,x]|

unsigned int tick = 0;

void start(int x, int y)
{
  while (x < y) {
    x++;
	tick++;
  }
  while (y < x) {
    y++;
	tick++;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(x, y);
	
	int bnd = (x>y)?(x-y):(y-x);
	assert(tick <= bnd);
	
	return 0;
}
