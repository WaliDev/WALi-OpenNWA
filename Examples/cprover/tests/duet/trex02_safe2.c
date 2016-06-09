//x is an input variable
int x;

void foo() {
  x--;
}

void main() {
    x=rand();
    assume(x >= 0);
    while (x > 0) {
	int c = rand();
	if(c) foo();
	else foo();
    }
    assert(x==0);
}



