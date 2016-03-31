unsigned int tick = 0;

void start(int n)
{
  int x=0;
  int y=0;

  for (;;) {
	tick++;
    if (x < n) {
      y=y+1;
      x=x+1;
    } else if (y > 0)
      y=y-1;
    else
      break;
  }
}

int main() 
{
	start(20);
	return 0;
}

