int main() {
	int x, y;
	x = y = 0;
	while(x < 10) {
		y = y + x*x;
		x++;
	}
	assert(6 * y == x - 3 * x * x + 2 * x*x*x);

	return 0;
}


