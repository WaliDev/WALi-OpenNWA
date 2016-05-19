void loop(int x) {
    if (x < 10) {
	loop(x + 1);
    } else {
	    assert(x >= 0); // safe
	    assert(x <= 10); // safe
	    assert(x == 10); // safe
    }
}

void main() {
    loop(0);
}
