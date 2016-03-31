unsigned int tick = 0;

void start(int n, int m)
{
  int x=0;
  int y=0;

  while (x<n) {
	tick++;
    if (y<m)
      y=y+1;
    else
      x=x+1;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
