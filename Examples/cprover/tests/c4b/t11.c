// C4B output: |[x,n]|+|[y,m]|

unsigned int tick = 0;

void start(int x, int y, int n, int m)
{
  while (n>x) {
	tick++;
    if (m>y) 
      y = y+1;
    else
      x = x+1;
  }
}

int main() 
{
	start(20, 30, 100, 500);
	return 0;
}
