int gas;
int memory[];
int localmem[];

int test(int addr0, int addr1, int call_dsize, int call_v, int calldata0, int sha1) {
	gas = 0;
	int r10, r11, r12, r13, r14, r15, r18, r19, r28, r3, r5, r7, r9;
	r10 = r11 = r12 = r13 = r14 = r15 = r18 = r19 = r28 = r3 = r5 = r7 = r9 = 0;
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
		r15 = 1691300792 == r12 ? 1 : 0;
		if (r15 != 0) {
			goto label4;
		} else {
			goto label12;
		}
	label4:
		gas = gas + (19);
		r5 = call_v == 0 ? 1 : 0;
		if (r5 != 0) {
			goto label5;
		} else {
			goto label11;
		}
	label5:
		gas = gas + (27);
		r3 = 0;
		goto label6;
	label6:
		gas = gas + (84);
		r9 = memory[0];
		r14 = r3 < r9 ? 1 : 0;
		r15 = r14 == 0 ? 1 : 0;
		if (r15 != 0) {
			goto label7;
		} else {
			goto label8;
		}
	label7:
		gas = gas + (12);
		return 0;
	label8:
		gas = gas + (84);
		r11 = memory[0];
		r13 = r3 < r11 ? 1 : 0;
		r14 = r13 == 0 ? 1 : 0;
		r15 = r14 == 0 ? 1 : 0;
		if (r15 != 0) {
			goto label9;
		} else {
			goto label10;
		}
	label9:
		gas = gas + (149);
		localmem[0] = 0;
		r14 = sha1;
		r18 = r3 + r14;
		r19 = memory[r18];
		gas = gas + (r19 == 0 ? 5000 : 20000);
		// Refund gas ignored
		memory[1] = r19;
		r28 = 1 + r3;
		r3 = r28;
		goto label6;
	label10:
		return 0;
	label11:
		gas = gas + (6);
		return 0;
	label12:
		gas = gas + (7);
		return 0;

}

void main(int addr0, int addr1, int call_dsize, int call_v, int calldata0, int sha1) {
	test(addr0, addr1, call_dsize, call_v, calldata0, sha1);
	__VERIFIER_print_hull(gas);
	return;
}
