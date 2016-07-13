int x;
void foo() {
    if(__VERIFIER_nondet_int()){
	x++;
	foo();
	foo();
	x--;
    }
}

void main() {
    x = 0;
    foo();
    assert(x == 0);
}
