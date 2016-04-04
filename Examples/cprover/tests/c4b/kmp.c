/* Knuth-Morris-Pratt string searching */
// C4B output: 1+2|[0,n]|

unsigned int tick = 0;

int srch(int t[], int n, int p[], int m, int b[])
{
	int i = 0, j = 0, k = -1;
	
	while (i < n) {
		while (j >= 0 && t[i] != p[j]) {
			tick++;
			k = b[j];
			assert(k > 0);
			assert(k <= j + 1);
			j -= k;
		}
		i++, j++;
		if (j == m)
			break;
	}
	return i;
}

int main()
{
	int p[20];
	int t[30];
	int b[40];
	srch(t, 5, p, 10, b);
	return 0;
}
