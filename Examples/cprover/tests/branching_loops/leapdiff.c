// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main(int N) {
    int x = 1;
    int y = 2;
    int z = 1;
    for(int t = 0; t < N; t++) {
        if (z > 0) {
            x = x + 2; 
            z = -1;
        } else { 
            y = y + 2; 
            z = 1;
        }
    }
    assert(x + z == y);
    //assert((-1 <= x - y) && (x - y <= 1));
}
