// C4B output: 1.33|[y,z]|+0.33|[0,y]|

unsigned int tick = 0;

void start(int y, int z)
{
  while (z>y) {
	tick++;
    y=y+1;
  }
  while (y>2) {
	tick++;
    y=y-3;
  }
}

int main() 
{
	int y = __VERIFIER_nondet_int();
	int z = __VERIFIER_nondet_int();

	start(y, z);
	
	int bnd = 4 * ((z>y)?(z-y):0) + ((y>0)?y:0);
	assert(3 * tick <= bnd);
	
	return 0;
}
