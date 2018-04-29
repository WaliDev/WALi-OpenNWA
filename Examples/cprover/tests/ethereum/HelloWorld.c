int gas;
int memory[];
int localmem[];
int test(int call_v) {
	gas = 0;
	int r6;
	r6 = 0;
	label1:
		gas = gas + (27);
		// UseMemory operations currently ignored
		localmem[64] = 96;
		r6 = call_v == 0 ? 1 : 0;
		if (r6 != 0) {
			goto label2;
		} else {
			goto label3;
		}
	label2:
		gas = gas + (52);
		// UseMemory operations currently ignored
		localmem[0] = 0;
		return 0;
	label3:
		gas = gas + (6);
		return 0;
}

void main(int call_v) {
	test(call_v);
	__VERIFIER_print_hull(gas);
	return;
}
