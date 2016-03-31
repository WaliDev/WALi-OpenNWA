unsigned int tick = 0;

void start(int x, int z, int n)
{
  while (x<n) {
	tick++;
    if (z>x)
      x=x+1;
    else
      z=z+1;
  }
}

int main() 
{
	start(20, 30, 100);
	return 0;
}
