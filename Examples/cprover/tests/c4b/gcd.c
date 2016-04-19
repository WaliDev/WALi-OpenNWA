// C4B output : |[0,x]|+|[0,y]|

unsigned int tick = 0;

int gcd(int x, int y) {
	// used for assertions
	int inX=x;
	int inY=y;

	if (x <= 0) return y;
	if (y <= 0) return x;
	for (;;) {
		tick++;
		if (x>y) x -= y;
		else if (y>x) y -= x;
		else break;
	}
	
	int bnd = (inX>0?inX:0) + (inY>0?inY:0);
	assert(tick <= bnd);
	int simpbnd = inX + inY;
	assert(tick <= simpbnd);
	
	return x;
}

void assertion (int x, int y) {
	tick = 0;
	gcd(x, y);
	
	int assertionbnd = (x>0?x:0) + (y>0?y:0);
	assert(tick <= assertionbnd);
}
	

int main() 
{
	int x = __VERIFIER_nondet_int();
	int y = __VERIFIER_nondet_int();
	int mainbnd = (x>0?x:0) + (y>0?y:0);

	assertion(x, y);
	
	assert(tick <= mainbnd);
	return 0;
}
