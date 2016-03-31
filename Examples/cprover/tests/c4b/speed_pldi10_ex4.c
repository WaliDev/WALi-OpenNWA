unsigned int tick = 0;

void start(int n)
{
  int flag=1;

  while (flag>0) {
    flag=0;
    while (n>0) {
	  tick++;
      n=n-1;       
      flag=1;
    }
  }
}

int main() 
{
	start(20);
	return 0;
}
