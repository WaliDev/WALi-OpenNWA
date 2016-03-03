int main() {
    int n = 11;
    int i = 0;
    int r = 1;

    for (; i < n; i++) {
	r = r + r;
    }

    assert(r == 2048);

    return 0;
}
 
