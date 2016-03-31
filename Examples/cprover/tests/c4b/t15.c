unsigned int tick = 0;

void start(int x, int y)
{
  int z;

  assert(y >= 0);

  while (x>y) {
	tick++;
    x-=y+1;
    for (z=y; z>0; z--)
      /* nothing */;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
