// C4B output: Not Available
// SPEED output: mx(n,n-m)

unsigned int tick = 0;

void start(int n, int m, int dir)
{
  int i;

  assert(0 < m);
  assert(m < n);

  i = m;

  while (0 < i && i < n) {
    tick++;
    if (dir == 1)
      i++;
    else
      i--;
  }
}

int main() 
{
	start(20, 30, 1);
	return 0;
}
