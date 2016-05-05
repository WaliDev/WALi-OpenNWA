// C4B output : |[0,x]|+|[0,y]|
int __VERIFIER_nondet_int();
unsigned int tick = 0;

int gcd(int x, int y) {

	if (x <= 0) return y;
	if (y <= 0) return x;
	for (;;) {
		tick++;
		if (x>y) x -= y;
		else if (y>x) y -= x;
		else break;
	}
	
	return x;
}

	

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();	

	gcd(x, y);
	
	int mainbnd = (x>0?x:0) + (y>0?y:0);
	assert(tick <= mainbnd);
	return 0;
}
