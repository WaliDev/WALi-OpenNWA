int loop(int x) {
    if (x < 10) {
	return loop(x + 1);
    } else {
	return x;
    }
}

void main() {
    int x;
    x = loop(0);
	assert(x >= 0); // safe
	assert(x <= 10); // safe
	assert(x == 10); // safe
}
