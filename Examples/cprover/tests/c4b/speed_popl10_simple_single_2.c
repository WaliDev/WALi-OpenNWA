int __VERIFIER_nondet_int();
unsigned int tick = 0;

void start(int n, int m)
{
  int x=0;
  int y=0;

  for (;;) {
	tick++;
    if (x<n) {
      x=x+1; y=y+1;
    }
    else if (y<m) {
      x=x+1; y=y+1;
    }
    else
      break;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
