unsigned int tick = 0;

void start(int i, int k)
{
  while (i > 100) {
	tick++;
    i--;
  }
  i=i+k+50;
  while (i >= 0) {
	tick++;
    i--;
  }
}

int main() 
{
	start(200, 300);
	return 0;
}
