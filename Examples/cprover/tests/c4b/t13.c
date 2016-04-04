// C4B output: 2|[0,x]|+|[0,y]|

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int x, int y)
{
  while (x > 0) {
    x=x-1;
    if (__VERIFIER_nondet_int()) {
	  tick++
      y=y+1;
	}
    else
      while (y > 0) {
		tick++;
        y=y-1;
	  }
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
