// C4B output: |[x,n]|+|[y,m]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int x, int n, int y, int m)
{
  while (x<n) {
    while (y<m) {
	  tick++;
      if (__VERIFIER_nondet_int())
        break;
      y=y+1;
    }
    x=x+1;
  }
}

int main() 
{
	start(20, 100, 30, 500);
	return 0;
}
