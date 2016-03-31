/* Quicksort */

int __VERIFIER_nondet_int();
unsigned int tick = 0;

void qsort(int *a, int lo, int hi) {
  tick++;
  int m1, m2, n;

  if (hi - lo < 1)
    return;

  n = __VERIFIER_nondet_int();   /* partition the array */
  assert( n > 0 );
  assert( lo + n <= hi );

  m1 = n + lo;
  m2 = m1 - 1;

  qsort(a, m1, hi);
  qsort(a, lo, m2);
}

void start(int *a, int len) {
  qsort(a, 0, len);
}

int main() 
{
	int a[20];
	start (a, 20);
	return 0;
}
