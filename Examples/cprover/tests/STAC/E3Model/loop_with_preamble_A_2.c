int M_responseLength;

void serve() {
    M_responseLength = 0;
    for(int M_ii = 0; M_ii < 4; M_ii++) {
        M_responseLength += 2;
    }
}

void main() {
    serve();
    __VERIFIER_print_hull(M_responseLength);
}
