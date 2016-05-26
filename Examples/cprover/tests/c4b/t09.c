// C4B output: |[0, x]|

unsigned int tick = 0;

void start(int x) {
	int i,j;
	i=1, j=0;
	while (j<x) {
		tick++;
		j++;
		if (i>=4) {
			i=1;
		} else
			i++;
	}
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	
	start(x);
	
	int bnd = (x>0?x:0);
	assert (tick <= bnd);
	return 0;
}
