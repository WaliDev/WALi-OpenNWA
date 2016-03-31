unsigned int tick = 0; 

void count_up(int, int);

void count_down(int x,int y)
{
  tick++;
  if (x>y) {
    x--;
    count_up(x, y);
  }
}

void count_up(int x, int y)
{
  tick++;
  if (y+1<x) {
    y += 2;
    count_down(x, y);
  }
}

void start(int y, int z) {
  tick++;
  count_down(y, z);
}

int main() 
{
	start(20, 30);
	return 0;
}
