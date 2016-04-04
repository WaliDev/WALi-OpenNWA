// C4B output: Not Available

unsigned int tick = 0;

void start(int x) {
	int i,j;
	i=1, j=0;
	while (j<x) {
		tick++;
		j++;
		if (i>=4) {
			i=1;
		} else
			i++;
	}
}

int main() 
{
	start(20);
	return 0;
}
