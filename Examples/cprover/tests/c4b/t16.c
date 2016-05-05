// C4B output: 101|[0,x]|

unsigned int tick = 0;

void start(int x, int y)
{
  int z;

//  assert(y >= 0);

  while (x > y) {
    x -= y+1;
    z = 100 + 2*y;
    while (z > 0) {
	  tick++;
      z--;
	}
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
	__VERIFIER_assume(y >= 0);

	start(x, y);
	
	int bnd = 101 * ((x>0)?x:0);
	assert(tick <= bnd);
	
	return 0;
}
