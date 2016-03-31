unsigned int tick = 0;

void start(int n, int m)
{
  int va=n;
  int vb=0;

/* assert(n > 0); */
  assert(m > 0);

  while (va > 0) {
	tick++;
    if (vb < m) { 
      vb=vb+1; 
      va=va-1;
    } else {
      vb=vb-1;
      vb=0;
    }
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
