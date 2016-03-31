unsigned int tick = 0;

void start(int x, int y)
{
  while (x > y) {
	tick++;
    x--;
    x+=1000;
    y+=1000;
  }

  while (y>0) {
	tick++;
    y--;
  }
  while (x<0) {
	tick++;
    x++;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
