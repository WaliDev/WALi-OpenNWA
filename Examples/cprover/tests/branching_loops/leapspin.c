// WARNING: The files in this directory have not been extensively tested
//   and may be incorrect. --JTB

void main(int N) {
    int d = 1;
    for(int t = 0; t < N; t++) {
        if (d == 1) {
            d = d - 2;
        } else if (d == -1) { 
            d = d + 2;
        } else {

        }
    }
    assert(d == 1 || d == -1);
}
