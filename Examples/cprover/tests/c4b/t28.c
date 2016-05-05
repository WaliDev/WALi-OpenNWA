// C4B output: |[x,0]|+|[0,y]|+1002|[y,x]|

unsigned int tick = 0;

void start(int x, int y)
{
  while (x > y) {
	tick++;
    x--;
    x+=1000;
    y+=1000;
  }

  while (y>0) {
	tick++;
    y--;
  }
  while (x<0) {
	tick++;
    x++;
  }
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(x, y);
	
	int bnd = ((x<0)?-x:0) + ((y>0)?y:0) + 1002 * ((x>y)?(x-y):0);
	assert(tick <= bnd);
	
	return 0;
}
