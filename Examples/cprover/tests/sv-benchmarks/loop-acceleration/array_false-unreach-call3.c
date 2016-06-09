#define N 1024

int main(void) {
  int A[N];
  int i;

  for (i = 0; A[i] != 0 && i < N; i++) {
  }

  __VERIFIER_assert(i <= N / 2);
}
