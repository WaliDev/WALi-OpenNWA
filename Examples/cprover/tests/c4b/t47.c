unsigned int tick = 0;

void start(int n)
{
  int flag = 1;

  while (flag>0) {
	tick++;
    if (n>0) {
      n--;
      flag=1;
    } else
      flag=0;
  }
}

int main() 
{
	start(20);
	return 0;
}
