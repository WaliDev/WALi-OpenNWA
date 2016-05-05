// C4B output: 3+2|[0,x]|+|[0,y]|

unsigned int tick = 0;

void count_down(int x) {
  tick++;
  if (x>0) {
    x--;
    count_down(x);
  }
}

int copy(int x, int y) {
  tick++;
  if (x>0) {
    y++;
    x--;
    y=copy(x, y);
  }
  return y;
}

void start(int x, int y) {
  tick++;
  y = copy(x, y);
  count_down(y);
}

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();

	start(x, y);
	
	int bnd = 3 + 2 * ((x>0)?x:0) + ((y>0)?y:0);
	assert(tick <= bnd);
	
	return 0;
}
