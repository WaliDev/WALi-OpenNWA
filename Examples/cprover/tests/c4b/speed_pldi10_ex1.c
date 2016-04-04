// C4B output : |[0,n]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int n)
{
  int i;
  int j;

  i=0;
  while (i<n) {
    j=i+1;
    while (j<n) {
	  tick++;
      if (__VERIFIER_nondet_int()) {
        j=j-1;
        n=n-1;
      }
      j=j+1;
    }
    i=i+1;
  }
}

int main() 
{
	start(20);
	return 0;
}
