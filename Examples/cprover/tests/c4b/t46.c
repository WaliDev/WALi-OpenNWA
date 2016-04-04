// C4B output: |[0,y]|

unsigned int tick = 0;

int x, y;

void produce() {
  while (x > 0) {
    tick++;	
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
	start(20, 30);
	return 0;
}
