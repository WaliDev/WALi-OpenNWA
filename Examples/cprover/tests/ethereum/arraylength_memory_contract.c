int gas;
int memory[];
int localmem[];
int test(int addr0, int addr1, int call_dsize, int call_v, int calldata0) {
	gas = 0;
	int r10, r12, r14, r15, r5, r7;
	r10 = r12 = r14 = r15 = r5 = r7 = 0;
	label1:
		gas = gas + (30);
		// UseMemory operations currently ignored
		localmem[64] = 96;
		r7 = call_dsize < 4 ? 1 : 0;
		if (r7 != 0) {
			goto label2;
		} else {
			goto label3;
		}
	label2:
		gas = gas + (7);
		return 0;
	label3:
		gas = gas + (45);
		r5 = calldata0;
		r10 = r5 / addr0;
		r12 = (addr1 & r10);
		r15 = 638722032 == r12 ? 1 : 0;
		if (r15 != 0) {
			goto label4;
		} else {
			goto label7;
		}
	label4:
		gas = gas + (19);
		r5 = call_v == 0 ? 1 : 0;
		if (r5 != 0) {
			goto label5;
		} else {
			goto label6;
		}
	label5:
		gas = gas + (159);
		r14 = memory[0];
		// UseMemory operations currently ignored
		localmem[96] = r14;
		return 0;
	label6:
		gas = gas + (6);
		return 0;
	label7:
		gas = gas + (7);
		return 0;

}

void main(int addr0, int addr1, int call_dsize, int call_v, int calldata0) {
	test(addr0, addr1, call_dsize, call_v, calldata0);
	__VERIFIER_print_hull(gas);
	return;
}
