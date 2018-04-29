int gas;
int memory[];
int localmem[];
int test(int addr0, int addr1, int call_dsize, int call_v, int calldata0, int calldata36, int calldata4) {
	gas = 0;
	int r10, r12, r13, r14, r15, r3, r30, r4, r5, r6, r7;
	r10 = r12 = r13 = r14 = r15 = r3 = r30 = r4 = r5 = r6 = r7 = 0;
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
		r15 = 332507694 == r12 ? 1 : 0;
		if (r15 != 0) {
			goto label4;
		} else {
			goto label13;
		}
	label4:
		gas = gas + (19);
		r5 = call_v == 0 ? 1 : 0;
		if (r5 != 0) {
			goto label5;
		} else {
			goto label12;
		}
	label5:
		gas = gas + (99);
		r14 = calldata4;
		r30 = calldata36;
		r3 = r14;
		r4 = r30;
		r6 = 0;
		r7 = 0;
		goto label6;
	label6:
		gas = gas + (26);
		r12 = r6 < r3 ? 1 : 0;
		r13 = r12 == 0 ? 1 : 0;
		if (r13 != 0) {
			goto label7;
		} else {
			goto label11;
		}
	label7:
		gas = gas + (1);
		goto label8;
	label8:
		gas = gas + (26);
		r12 = r7 < r4 ? 1 : 0;
		r13 = r12 == 0 ? 1 : 0;
		if (r13 != 0) {
			goto label9;
		} else {
			goto label10;
		}
	label9:
		gas = gas + (87);
		r12 = r6 + r7;
		// UseMemory operations currently ignored
		localmem[96] = r12;
		return 0;
	label10:
		gas = gas + (25);
		r12 = 1 + r7;
		r7 = r12;
		goto label8;
	label11:
		gas = gas + (25);
		r12 = 1 + r6;
		r6 = r12;
		goto label6;
	label12:
		gas = gas + (6);
		return 0;
	label13:
		gas = gas + (7);
		return 0;

}

void main(int addr0, int addr1, int call_dsize, int call_v, int calldata0, int calldata36, int calldata4) {
	test(addr0, addr1, call_dsize, call_v, calldata0, calldata36, calldata4);
	__VERIFIER_print_hull(gas);
	return;
}
