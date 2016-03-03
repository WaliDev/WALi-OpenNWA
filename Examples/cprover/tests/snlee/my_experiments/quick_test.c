int main() {
	int x = 0;
	int y = -2;

	int n;
	while(x < n) {
		y = y + x;
		x = x + 1;
	}

	assert(2 * (y + 2) == x * (x - 1));

	return 0;
}

