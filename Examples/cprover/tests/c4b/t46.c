// C4B output: |[0,y]|

unsigned int tick = 0;

int x, y;

void produce() {
  while (x > 0) {
    tick--;	
	//tick(-1);
    x--; y++;
  }
}

void consume() {
  while (y > 0) {
	tick++;
    y--; x++; 
	//tick(1);
  }
}

void start(int y, int z) {
  consume(); produce(); consume();
}

int main() 
{
	x = __VERIFIER_nondet_int();
	y = __VERIFIER_nondet_int();
	int z = __VERIFIER_nondet_int();

	int bnd = (y>0)?y:0;
	
	start(y, z);
	
	assert(tick <= bnd);
	
	return 0;
}
