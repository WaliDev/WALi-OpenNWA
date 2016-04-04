// C4B output: 1+3|[0,x]|+|[0,y]|

unsigned int tick = 0;

void fish() { return; }

void start(int x, int y)
{
  while (x>0) {
	tick++;
    x=x-1;
    y=y+2;
  }
  while (y>0) {
	tick++;
    y=y-1;
  }
  fish();
  while (y>0) {
	tick++;
    y=y-1;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
