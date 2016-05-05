// C4B output: 50+|[-1,i]|+|[0,k]|

unsigned int tick = 0;

void start(int i, int k)
{
  while (i > 100) {
	tick++;
    i--;
  }
  i=i+k+50;
  while (i >= 0) {
	tick++;
    i--;
  }
}

int main() 
{
	int i = __VERIFIER_nondet_int();
	int k = __VERIFIER_nondet_int();

	start(i, k);
	
	int bnd = 50 + ((i>-1)?(i+1):0) + ((k>0)?k:0);
	assert(tick <= bnd);
	
	return 0;
}
