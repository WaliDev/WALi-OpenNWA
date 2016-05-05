// C4B output: 1+|[0,n]|

unsigned int tick = 0;

void start(int n)
{
  int flag = 1;

  while (flag>0) {
	tick++;
    if (n>0) {
      n--;
      flag=1;
    } else
      flag=0;
  }
}

int main() 
{
	int n = __VERIFIER_nondet_int();
	
	start(n);
	
	int bnd = 1 + ((n>0)?n:0);
	assert (n <= bnd);
	
	return 0;
}
