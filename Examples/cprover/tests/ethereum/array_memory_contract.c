int gas;
int memory[];
int localmem[];
int test(int addr0, int addr1, int addr2, int call_dsize, int call_v, int calldata0, int sha1) {
	gas = 0;
	int r10, r12, r13, r14, r15, r20, r26, r27, r5, r51, r60, r65, r7, r8;
	r10 = r12 = r13 = r14 = r15 = r20 = r26 = r27 = r5 = r51 = r60 = r65 = r7 = r8 = 0;
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
			goto label17;
		}
	label4:
		gas = gas + (19);
		r5 = call_v == 0 ? 1 : 0;
		if (r5 != 0) {
			goto label5;
		} else {
			goto label16;
		}
	label5:
		gas = gas + (46);
		goto label15;
	label6:
		gas = gas + (236);
		// UseMemory operations currently ignored
		localmem[96] = 0;
		localmem[64] = 128;
		r51 = memory[0];
		gas = gas + (0 == 0 ? 5000 : 20000);
		// Refund gas ignored
		memory[0] = 0;
		localmem[0] = 0;
		r60 = sha1;
		r65 = r60 + r51;
		r5 = r65;
		r7 = r60;
		goto label7;
	label7:
		gas = gas + (38);
		r26 = r5;
		r27 = r7;
		r7 = r5;
		r8 = r27;
		goto label8;
	label8:
		gas = gas + (26);
		r13 = r7 > r8 ? 1 : 0;
		r14 = r13 == 0 ? 1 : 0;
		if (r14 != 0) {
			goto label9;
		} else {
			goto label10;
		}
	label9:
		gas = gas + (52);
		return 0;
	label10:
		gas = gas + (31);
		gas = gas + (0 == 0 ? 5000 : 20000);
		// Refund gas ignored
		memory[r8] = 0;
		r20 = 1 + r8;
		r8 = r20;
		goto label8;
	label11:
		return 0;
	label12:
		return 0;
	label13:
		return 0;
	label14:
		return 0;
	label15:
		gas = gas + (4);
		goto label6;
	label16:
		gas = gas + (6);
		return 0;
	label17:
		gas = gas + (7);
		return 0;

}

void main(int addr0, int addr1, int addr2, int call_dsize, int call_v, int calldata0, int sha1) {
	test(addr0, addr1, addr2, call_dsize, call_v, calldata0, sha1);
	__VERIFIER_print_hull(gas);
	return;
}
