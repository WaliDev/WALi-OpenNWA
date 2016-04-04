// C4B output: |[x,y]|+|[y,x]|

unsigned int tick = 0;

void start(int x, int y)
{
  while (x < y) {
    x++;
	tick++;
  }
  while (y < x) {
    y++;
	tick++;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
