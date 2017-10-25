// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main(int N) {
    int x = 2;
    int y = 1;
    for(int t = 0; t < N; t++) {
        if (x - y == 1) {
            y = y + 2; 
        } else if (y - x == 1) { 
            x = x + 2; 
        } else {

        }
    }
    assert(x - y == 1 || y - x == 1);
}
