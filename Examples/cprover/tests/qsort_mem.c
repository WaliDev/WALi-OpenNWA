// adapted from the C4B example "qsort.c"
#define LENGTH 20

void qsort(int *a, int lo, int hi) {

	int m1, m2, n;

    // Here, we assert that the part of the array we're working on (lo...hi)
    // stays within the bounds of the array (0...LENGTH)
    assert(lo >= 0);
    assert(hi <= LENGTH);

	if (hi - lo < 1) return;

	n = __VERIFIER_nondet_int();	 
	__VERIFIER_assume(n > 0);
	__VERIFIER_assume(lo + n <= hi);

    // (The actual sorting work is omitted.)

	m1 = n + lo;
	m2 = m1 - 1;

	qsort(a, m1, hi);
	qsort(a, lo, m2);
}

int main()
{
	int a[LENGTH];
	int i;
	for (i = 0; i < LENGTH; i++)
		a[i] = __VERIFIER_nondet_int();

    qsort(a, 0, LENGTH);
	return 0;
}
