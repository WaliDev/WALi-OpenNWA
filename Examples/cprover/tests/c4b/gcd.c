// C4B output : |[0,x]|+|[0,y]|

unsigned int tick = 0;

int gcd(int x, int y) {
	if (x <= 0) return y;
	if (y <= 0) return x;
	for (;;) {
		tick++;
		if (x>y) x -= y;
		else if (y>x) y -= x;
		else return x;
  }
}

int main() 
{
	gcd(20, 30);
	return 0;
}
