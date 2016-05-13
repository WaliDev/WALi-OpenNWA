// C4B output: 1.33+0.67|[z,y]|

unsigned int tick = 0; 

void count_up(int, int);

void count_down(int x,int y)
{
  tick++;
  if (x>y) {
    x--;
    count_up(x, y);
  }
}

void count_up(int x, int y)
{
  tick++;
  if (y+1<x) {
    y += 2;
    count_down(x, y);
  }
}

void start(int y, int z) {
  count_down(y, z);
}

int main() 
{
	int y = __VERIFIER_nondet_int();
	int z = __VERIFIER_nondet_int();

	start(y, z);
	
	int bnd = 4 + 2 * ((y>z)?(y-z):0);
	assert(3 * tick <= bnd);
	
	return 0;
}
