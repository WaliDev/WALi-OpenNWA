int M_responseLength;

void serve() {
    M_responseLength = 0;
    if (__VERIFIER_nondet_int()) {
        M_responseLength = 30;
        return;
    }
    for(int M_ii = 0; M_ii < 4; M_ii++) {
        M_responseLength += 1;
    }
}

void main() {
    serve();
    __VERIFIER_print_hull(M_responseLength);
}
