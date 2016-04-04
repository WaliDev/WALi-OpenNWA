// C4B output: 101|[0,x]|

unsigned int tick = 0;

void start(int x, int y)
{
  int z;

  assert(y >= 0);

  while (x > y) {
    x -= y+1;
    z = 100 + 2*y;
    while (z > 0) {
	  tick++;
      z--;
	}
  }
}

int main() 
{
	start(20, 30);
	return 0;
}
