// C4B output: 1.33|[y,z]|+0.33|[0,y]|

unsigned int tick = 0;

void start(int y, int z)
{
  while (z>y) {
	tick++;
    y=y+1;
  }
  while (y>2) {
	tick++;
    y=y-3;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
