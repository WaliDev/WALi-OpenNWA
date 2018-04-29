int gas;
int memory[];
int localmem[];
int test(int addr0, int addr1, int addr2, int call_dsize, int call_v, int calldata0, int calldata4) {
	gas = 0;
	int r10, r11, r12, r13, r15, r3, r5, r6, r7;
	r10 = r11 = r12 = r13 = r15 = r3 = r5 = r6 = r7 = 0;
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
		r15 = addr2 == r12 ? 1 : 0;
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
		gas = gas + (67);
		r13 = calldata4;
		r3 = r13;
		r5 = 0;
		goto label6;
	label6:
		gas = gas + (26);
		r11 = r5 < r3 ? 1 : 0;
		r12 = r11 == 0 ? 1 : 0;
		if (r12 != 0) {
			goto label7;
		} else {
			goto label8;
		}
	label7:
		gas = gas + (79);
		// UseMemory operations currently ignored
		localmem[96] = r5;
		return 0;
	label8:
		gas = gas + (8);
		r6 = 0;
		goto label9;
	label9:
		gas = gas + (26);
		r11 = r6 < r3 ? 1 : 0;
		r12 = r11 == 0 ? 1 : 0;
		if (r12 != 0) {
			goto label10;
		} else {
			goto label11;
		}
	label10:
		gas = gas + (26);
		r11 = 1 + r5;
		r5 = r11;
		goto label6;
	label11:
		gas = gas + (25);
		r11 = 1 + r6;
		r6 = r11;
		goto label9;
	label12:
		gas = gas + (6);
		return 0;
	label13:
		gas = gas + (7);
		return 0;

}

void main(int addr0, int addr1, int addr2, int call_dsize, int call_v, int calldata0, int calldata4) {
	test(addr0, addr1, addr2, call_dsize, call_v, calldata0, calldata4);
	__VERIFIER_print_hull(gas);
	return;
}
