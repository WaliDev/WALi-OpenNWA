int x;
void loop() {
    if (x < 10) {
	x = x + 1;
	loop();
    }
}

void main() {
    x = 0;
    loop();
	assert(x >= 0); // safe
	assert(x <= 10); // safe
	assert(x == 10); // safe
}
