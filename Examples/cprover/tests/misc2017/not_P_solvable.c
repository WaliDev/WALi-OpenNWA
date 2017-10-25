void main(int N, int x, int initial) {
    int y = initial, z = initial;
    for(int i = 0; i < N; i++) {
        x = x * x;
        y = y + x;
        z = z + x;
    }
    assert(y == z);
}
