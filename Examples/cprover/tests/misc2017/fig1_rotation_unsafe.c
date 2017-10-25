void main() {
    int x = -1; 
    int y = 0;
    int temp = 0;
    
    for(int k = 0; k < 1; k++) {
        temp = x; x = y; y = -temp;
    }
    
    __VERIFIER_assert( y != 1 );   // this is a false assertion
    //if (y == 1) goto errorlabel; // this is the original form (it's also a false assertion)
}
