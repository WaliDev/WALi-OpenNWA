int gas;
int localmem[];
int memory[];
int test(int addr0, int addr1, int addr2, int addr3, int call_dsize, int call_v, int calldata0, int calldata36, int calldata4, int sha1, int sha2, int sha3) {
	gas = 0;
	int r1, r10, r12, r13, r14, r15, r18, r19, r20, r26, r27, r30, r33, r35, r36, r37, r5, r50, r51, r52, r53, r54, r6, r60, r65, r7, r8;
	r1 = r10 = r12 = r13 = r14 = r15 = r18 = r19 = r20 = r26 = r27 = r30 = r33 = r35 = r36 = r37 = r5 = r50 = r51 = r52 = r53 = r54 = r6 = r60 = r65 = r7 = r8 = 0;
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
		r15 = 447770341 == r12 ? 1 : 0;
		r1 = r12;
		if (r15 != 0) {
			goto label4;
		} else {
			goto label9;
		}
	label4:
		gas = gas + (19);
		r5 = call_v == 0 ? 1 : 0;
		if (r5 != 0) {
			goto label5;
		} else {
			goto label8;
		}
	label5:
		gas = gas + (161);
		r14 = calldata4;
		r30 = calldata36;
		r50 = memory[0];
		r52 = r14 < r50 ? 1 : 0;
		r53 = r52 == 0 ? 1 : 0;
		r54 = r53 == 0 ? 1 : 0;
		r5 = r30;
		r7 = r14;
		if (r54 != 0) {
			goto label6;
		} else {
			goto label7;
		}
	label6:
		gas = gas + (79);
		localmem[0] = 0;
		r15 = sha1;
		r19 = r7 + r15;
		gas = gas + (r5 == 0 ? 5000 : 20000);
		// Refund gas ignored
		memory[r19] = r5;
		return 0;
	label7:
		return 0;
	label8:
		gas = gas + (6);
		return 0;
	label9:
		gas = gas + (22);
		r6 = addr2 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label10;
		} else {
			goto label15;
		}
	label10:
		gas = gas + (19);
		r5 = call_v == 0 ? 1 : 0;
		if (r5 != 0) {
			goto label11;
		} else {
			goto label14;
		}
	label11:
		gas = gas + (137);
		r13 = calldata4;
		r33 = memory[0];
		r35 = r13 < r33 ? 1 : 0;
		r36 = r35 == 0 ? 1 : 0;
		r37 = r36 == 0 ? 1 : 0;
		r6 = r13;
		if (r37 != 0) {
			goto label12;
		} else {
			goto label13;
		}
	label12:
		gas = gas + (179);
		localmem[0] = 0;
		r14 = sha2;
		r18 = r6 + r14;
		r19 = memory[r18];
		// UseMemory operations currently ignored
		localmem[96] = r19;
		return 0;
	label13:
		return 0;
	label14:
		gas = gas + (6);
		return 0;
	label15:
		gas = gas + (22);
		r6 = addr3 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label16;
		} else {
			goto label29;
		}
	label16:
		gas = gas + (19);
		r5 = call_v == 0 ? 1 : 0;
		if (r5 != 0) {
			goto label17;
		} else {
			goto label28;
		}
	label17:
		gas = gas + (46);
		goto label27;
	label18:
		gas = gas + (236);
		// UseMemory operations currently ignored
		localmem[96] = 0;
		localmem[64] = 128;
		r51 = memory[0];
		gas = gas + (0 == 0 ? 5000 : 20000);
		// Refund gas ignored
		memory[0] = 0;
		localmem[0] = 0;
		r60 = sha3;
		r65 = r60 + r51;
		r5 = r65;
		r7 = r60;
		goto label19;
	label19:
		gas = gas + (38);
		r26 = r5;
		r27 = r7;
		r7 = r5;
		r8 = r27;
		goto label20;
	label20:
		gas = gas + (26);
		r13 = r7 > r8 ? 1 : 0;
		r14 = r13 == 0 ? 1 : 0;
		if (r14 != 0) {
			goto label21;
		} else {
			goto label22;
		}
	label21:
		gas = gas + (52);
		return 0;
	label22:
		gas = gas + (31);
		gas = gas + (0 == 0 ? 5000 : 20000);
		// Refund gas ignored
		memory[r8] = 0;
		r20 = 1 + r8;
		r8 = r20;
		goto label20;
	label23:
		return 0;
	label24:
		return 0;
	label25:
		return 0;
	label26:
		return 0;
	label27:
		gas = gas + (4);
		goto label18;
	label28:
		gas = gas + (6);
		return 0;
	label29:
		gas = gas + (7);
		return 0;

}

void main(int addr0, int addr1, int addr2, int addr3, int call_dsize, int call_v, int calldata0, int calldata36, int calldata4, int sha1, int sha2, int sha3) {
	test(addr0, addr1, addr2, addr3, call_dsize, call_v, calldata0, calldata36, calldata4, sha1, sha2, sha3);
	__VERIFIER_print_hull(gas);
	return;
}
