//// C4B output : |[0,n]|

unsigned int tick = 0;

void start(int n, int m)
{
  int i=n;

  assert(0 < m);

  while (i > 0) {
	tick++;
    if (i < m)
      i=i-1;
    else
      i=i-m;
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
