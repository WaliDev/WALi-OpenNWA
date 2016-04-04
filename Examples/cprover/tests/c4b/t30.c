// C4B output: |[0,x]|+|[0,y]|

unsigned int tick = 0;

void start(int x, int y)
{
  int t;

  while (x>0) {
	tick++;
    x--;
    t=x, x=y, y=t;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
