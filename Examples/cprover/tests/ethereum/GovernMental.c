int __cost;
int memory[];
int localmem[];
int main(int addr0, int addr1, int addr10, int addr11, int addr12, int addr13, int addr14, int addr15, int addr2, int addr3, int addr4, int addr5, int addr6, int addr7, int addr8, int addr9, int balanceaddress, int call_dsize, int call_v, int calldata0, int calldata4, int caller, int sha1, int sha10, int sha11, int sha12, int sha13, int sha14, int sha15, int sha16, int sha17, int sha18, int sha19, int sha2, int sha20, int sha21, int sha22, int sha23, int sha3, int sha4, int sha5, int sha6, int sha7, int sha8, int sha9, int timestamp) {
	__cost = 0;
	int r1, r10, r101, r102, r103, r104, r105, r106, r107, r108, r109, r11, r112, r113, r12, r122, r123, r124, r125, r126, r128, r129, r13, r139, r14, r140, r141, r142, r143, r144, r145, r15, r159, r16, r160, r164, r165, r166, r17, r174, r175, r18, r187, r188, r189, r19, r2, r20, r206, r207, r208, r21, r22, r220, r221, r222, r227, r228, r23, r24, r241, r242, r243, r244, r25, r253, r254, r26, r261, r262, r264, r265, r266, r267, r27, r278, r279, r28, r280, r29, r3, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r4, r40, r41, r42, r43, r44, r45, r46, r47, r48, r49, r50, r51, r53, r54, r55, r56, r57, r58, r6, r60, r61, r62, r63, r64, r65, r66, r67, r69, r7, r70, r74, r75, r76, r77, r78, r79, r8, r81, r82, r83, r84, r85, r86, r87, r9, r90, r91, r92, r93;
	r1 = r10 = r101 = r102 = r103 = r104 = r105 = r106 = r107 = r108 = r109 = r11 = r112 = r113 = r12 = r122 = r123 = r124 = r125 = r126 = r128 = r129 = r13 = r139 = r14 = r140 = r141 = r142 = r143 = r144 = r145 = r15 = r159 = r16 = r160 = r164 = r165 = r166 = r17 = r174 = r175 = r18 = r187 = r188 = r189 = r19 = r2 = r20 = r206 = r207 = r208 = r21 = r22 = r220 = r221 = r222 = r227 = r228 = r23 = r24 = r241 = r242 = r243 = r244 = r25 = r253 = r254 = r26 = r261 = r262 = r264 = r265 = r266 = r267 = r27 = r278 = r279 = r28 = r280 = r29 = r3 = r30 = r31 = r32 = r33 = r34 = r35 = r36 = r37 = r38 = r39 = r4 = r40 = r41 = r42 = r43 = r44 = r45 = r46 = r47 = r48 = r49 = r50 = r51 = r53 = r54 = r55 = r56 = r57 = r58 = r6 = r60 = r61 = r62 = r63 = r64 = r65 = r66 = r67 = r69 = r7 = r70 = r74 = r75 = r76 = r77 = r78 = r79 = r8 = r81 = r82 = r83 = r84 = r85 = r86 = r87 = r9 = r90 = r91 = r92 = r93 = 0;
	label1:
		__cost = __cost + (27);
		// UseMemory operations currently ignored
		localmem[64] = 96;
		r6 = call_dsize == 0 ? 1 : 0;
		if (r6 != 0) {
			goto label2;
		} else {
			goto label42;
		}
	label2:
		__cost = __cost + (120);
		r24 = memory[1];
		r25 = r24 + 43200;
		r26 = r25 < timestamp ? 1 : 0;
		r27 = r26 == 0 ? 1 : 0;
		if (r27 != 0) {
			goto label3;
		} else {
			goto label23;
		}
	label3:
		__cost = __cost + (23);
		r10 = call_v < addr0 ? 1 : 0;
		if (r10 != 0) {
			goto label4;
		} else {
			goto label6;
		}
	label4:
		__cost = __cost + (80);
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (30);
		goto label5;
	label5:
		__cost = __cost + (31);
		goto exit;
	label6:
		__cost = __cost + (128);
		__cost = __cost + (timestamp == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[1] = timestamp;
		r23 = memory[3];
		r28 = r23 + 1;
		__cost = __cost + (r28 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[3] = r28;
		r44 = r23 == 0 ? 1 : 0;
		r49 = r44 > r28 ? 1 : 0;
		r7 = r23;
		r10 = r28;
		r11 = r23;
		if (r49 != 0) {
			goto label7;
		} else {
			goto label19;
		}
	label7:
		__cost = __cost + (276);
		localmem[0] = 3;
		r27 = sha1;
		r28 = r27 + r7;
		r30 = memory[r28];
		r37 = (addr1 & r30) == 0 ? 1 : 0;
		r39 = (caller | r37) == 0 ? 1 : 0;
		__cost = __cost + (r39 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r28] = r39;
		r45 = memory[4];
		r48 = r45 + 1;
		__cost = __cost + (r48 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[4] = r48;
		r55 = r45 == 0 ? 1 : 0;
		r60 = r55 > r48 ? 1 : 0;
		r7 = r45;
		r10 = r48;
		r11 = r45;
		if (r60 != 0) {
			goto label8;
		} else {
			goto label15;
		}
	label8:
		__cost = __cost + (243);
		localmem[0] = 4;
		r24 = sha2;
		r28 = call_v * 110;
		r33 = r28 / 100;
		r43 = r7 + r24;
		__cost = __cost + (r33 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r43] = r33;
		r48 = memory[5];
		r54 = (addr2 & r48) == 0 ? 1 : 0;
		r65 = call_v * 5;
		r66 = r65 / 100;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (93);
		r84 = memory[2];
		r85 = r84 < addr3 ? 1 : 0;
		r86 = r85 == 0 ? 1 : 0;
		if (r86 != 0) {
			goto label9;
		} else {
			goto label14;
		}
	label9:
		__cost = __cost + (180);
		localmem[0] = 0;
		localmem[32] = 6;
		r27 = sha3;
		r28 = memory[r27];
		r33 = r28 < call_v ? 1 : 0;
		if (r33 != 0) {
			goto label10;
		} else {
			goto label13;
		}
	label10:
		__cost = __cost + (430);
		localmem[0] = caller;
		localmem[32] = 6;
		r30 = sha4;
		r32 = memory[r30];
		r36 = call_v * 110;
		r37 = r36 / 100;
		r38 = r37 + r32;
		__cost = __cost + (r38 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r30] = r38;
		r42 = memory[0];
		r44 = memory[2];
		r47 = memory[4];
		r56 = balanceaddress;
		r66 = r56 - r44;
		r77 = (addr4 & r42) == 0 ? 1 : 0;
		r82 = r77 < r47 ? 1 : 0;
		r83 = r82 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (120);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r106 = addr5 + r77;
		r107 = memory[r106];
		r108 = r107 > r66 ? 1 : 0;
		if (r108 != 0) {
			goto label11;
		} else {
			goto label12;
		}
	label11:
		__cost = __cost + (20);
		goto label5;
	label12:
		__cost = __cost + (146);
		r18 = memory[0];
		r21 = memory[3];
		r29 = (addr4 & r18) == 0 ? 1 : 0;
		r34 = r29 < r21 ? 1 : 0;
		r35 = r34 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (376);
		localmem[0] = 3;
		localmem[0] = addr6;
		localmem[0] = 3;
		r53 = addr6 + r29;
		r60 = memory[r53];
		r75 = (addr2 & r60) == 0 ? 1 : 0;
		r81 = (addr2 & r75) == 0 ? 1 : 0;
		r90 = memory[0];
		r101 = (addr4 & r90) == 0 ? 1 : 0;
		r103 = memory[4];
		r105 = r101 < r103 ? 1 : 0;
		r106 = r105 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (130);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r123 = addr5 + r101;
		r125 = memory[r123];
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (163);
		r139 = memory[0];
		r142 = memory[4];
		r159 = (addr4 & r139) == 0 ? 1 : 0;
		r164 = r159 < r142 ? 1 : 0;
		r165 = r164 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (200);
		localmem[0] = 4;
		r174 = memory[3];
		localmem[0] = addr5;
		localmem[0] = 4;
		r187 = r159 + addr5;
		r188 = memory[r187];
		r206 = r159 < r174 ? 1 : 0;
		r207 = r206 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (358);
		localmem[0] = addr6;
		localmem[0] = 4;
		r220 = addr6 + r159;
		r221 = memory[r220];
		r227 = (addr2 & r221) == 0 ? 1 : 0;
		localmem[0] = r227;
		localmem[32] = 6;
		r241 = sha5;
		r243 = memory[r241];
		r253 = r243 - r188;
		__cost = __cost + (r253 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r241] = r253;
		r261 = memory[0];
		r264 = (r261 & addr4) == 0 ? 1 : 0;
		r266 = 1 + r264;
		r278 = (addr7 & r261) == 0 ? 1 : 0;
		r279 = (r278 | r266) == 0 ? 1 : 0;
		__cost = __cost + (r279 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[0] = r279;
		goto label11;
	label13:
		__cost = __cost + (96);
		r36 = call_v * 5;
		r37 = r36 / 100;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (10);
		goto label10;
	label14:
		__cost = __cost + (81);
		r13 = memory[2];
		r17 = call_v * 5;
		r18 = r17 / 100;
		r19 = r18 + r13;
		__cost = __cost + (r19 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[2] = r19;
		goto label9;
	label15:
		__cost = __cost + (89);
		localmem[0] = 4;
		r20 = sha6;
		r25 = r20 + r11;
		r29 = r20 + r10;
		r12 = r25;
		r13 = r29;
		goto label16;
	label16:
		__cost = __cost + (26);
		r18 = r12 > r13 ? 1 : 0;
		r19 = r18 == 0 ? 1 : 0;
		if (r19 != 0) {
			goto label17;
		} else {
			goto label18;
		}
	label17:
		__cost = __cost + (14);
		goto label8;
	label18:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r13] = 0;
		r19 = 1 + r13;
		r13 = r19;
		goto label16;
	label19:
		__cost = __cost + (89);
		localmem[0] = 3;
		r20 = sha7;
		r25 = r20 + r11;
		r29 = r20 + r10;
		r12 = r25;
		r13 = r29;
		goto label20;
	label20:
		__cost = __cost + (26);
		r18 = r12 > r13 ? 1 : 0;
		r19 = r18 == 0 ? 1 : 0;
		if (r19 != 0) {
			goto label21;
		} else {
			goto label22;
		}
	label21:
		__cost = __cost + (14);
		goto label7;
	label22:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r13] = 0;
		r19 = 1 + r13;
		r13 = r19;
		goto label20;
	label23:
		__cost = __cost + (79);
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (112);
		r47 = memory[3];
		r57 = r47 + -1;
		r62 = r57 < r47 ? 1 : 0;
		r63 = r62 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (253);
		r69 = memory[3];
		localmem[0] = 3;
		r75 = memory[2];
		r85 = r69 + addr8;
		r86 = memory[r85];
		r92 = (addr2 & r86) == 0 ? 1 : 0;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (152);
		r112 = memory[5];
		r122 = (addr2 & r112) == 0 ? 1 : 0;
		r128 = balanceaddress;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (120);
		r141 = memory[0];
		r144 = (addr7 & r141) == 0 ? 1 : 0;
		__cost = __cost + (r144 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[0] = r144;
		__cost = __cost + (timestamp == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[1] = timestamp;
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[2] = 0;
		goto label41;
	label24:
		__cost = __cost + (198);
		// UseMemory operations currently ignored
		localmem[96] = 0;
		localmem[64] = 128;
		r31 = memory[3];
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[3] = 0;
		localmem[0] = 3;
		localmem[0] = addr6;
		localmem[0] = 3;
		r62 = addr6 + r31;
		r7 = r62;
		r9 = addr6;
		goto label25;
	label25:
		__cost = __cost + (14);
		r17 = r7;
		r8 = r7;
		goto label26;
	label26:
		__cost = __cost + (26);
		r14 = r8 > r9 ? 1 : 0;
		r15 = r14 == 0 ? 1 : 0;
		if (r15 != 0) {
			goto label27;
		} else {
			goto label37;
		}
	label27:
		__cost = __cost + (49);
		goto label36;
	label28:
		__cost = __cost + (198);
		// UseMemory operations currently ignored
		localmem[128] = 0;
		localmem[64] = 160;
		r31 = memory[4];
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[4] = 0;
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r62 = addr5 + r31;
		r7 = r62;
		r9 = addr5;
		goto label29;
	label29:
		__cost = __cost + (14);
		r17 = r7;
		r8 = r7;
		goto label30;
	label30:
		__cost = __cost + (26);
		r14 = r8 > r9 ? 1 : 0;
		r15 = r14 == 0 ? 1 : 0;
		if (r15 != 0) {
			goto label31;
		} else {
			goto label32;
		}
	label31:
		__cost = __cost + (119);
		r16 = memory[7];
		r20 = (r16 & -256) == 0 ? 1 : 0;
		r28 = (r16 & 255) == 0 ? 1 : 0;
		r30 = 1 + r28;
		r31 = (r30 | r20) == 0 ? 1 : 0;
		__cost = __cost + (r31 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[7] = r31;
		goto label5;
	label32:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r9] = 0;
		r15 = 1 + r9;
		r9 = r15;
		goto label30;
	label33:
		goto exit;
	label34:
		goto exit;
	label35:
		goto exit;
	label36:
		__cost = __cost + (4);
		goto label28;
	label37:
		__cost = __cost + (111);
		r15 = memory[r9];
		r22 = (addr1 & r15) == 0 ? 1 : 0;
		__cost = __cost + (r22 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r9] = r22;
		r25 = 1 + r9;
		r9 = r25;
		goto label26;
	label38:
		goto exit;
	label39:
		goto exit;
	label40:
		goto exit;
	label41:
		__cost = __cost + (4);
		goto label24;
	label42:
		__cost = __cost + (59);
		r8 = calldata0;
		r9 = r8 / addr9;
		r12 = r9 == 166930775 ? 1 : 0;
		r1 = r9;
		if (r12 != 0) {
			goto label43;
		} else {
			goto label44;
		}
	label43:
		__cost = __cost + (126);
		r10 = memory[0];
		r12 = (addr4 & r10) == 0 ? 1 : 0;
		r24 = (r12 & addr4) == 0 ? 1 : 0;
		// UseMemory operations currently ignored
		localmem[96] = r24;
		goto exit;
	label44:
		__cost = __cost + (22);
		r6 = 342664497 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label45;
		} else {
			goto label46;
		}
	label45:
		__cost = __cost + (129);
		r10 = memory[7];
		r12 = (255 & r10) == 0 ? 1 : 0;
		r27 = (255 & r12) == 0 ? 1 : 0;
		// UseMemory operations currently ignored
		localmem[96] = r27;
		goto exit;
	label46:
		__cost = __cost + (22);
		r6 = 1005495957 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label47;
		} else {
			goto label49;
		}
	label47:
		__cost = __cost + (103);
		r9 = memory[5];
		r15 = (addr2 & r9) == 0 ? 1 : 0;
		r3 = r15;
		goto label48;
	label48:
		__cost = __cost + (84);
		r26 = (addr2 & r3) == 0 ? 1 : 0;
		// UseMemory operations currently ignored
		localmem[96] = r26;
		goto exit;
	label49:
		__cost = __cost + (22);
		r6 = 1167333944 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label50;
		} else {
			goto label52;
		}
	label50:
		__cost = __cost + (68);
		r7 = memory[1];
		r3 = r7;
		goto label51;
	label51:
		__cost = __cost + (43);
		// UseMemory operations currently ignored
		localmem[96] = r3;
		goto exit;
	label52:
		__cost = __cost + (22);
		r6 = 1243548662 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label53;
		} else {
			goto label57;
		}
	label53:
		__cost = __cost + (137);
		r10 = calldata4;
		r12 = memory[5];
		r22 = (addr2 & r12) == 0 ? 1 : 0;
		r34 = caller == r22 ? 1 : 0;
		r35 = r34 == 0 ? 1 : 0;
		r3 = r10;
		if (r35 != 0) {
			goto label54;
		} else {
			goto label56;
		}
	label54:
		__cost = __cost + (11);
		goto label55;
	label55:
		__cost = __cost + (1);
		goto exit;
	label56:
		__cost = __cost + (113);
		r11 = memory[5];
		r18 = (addr1 & r11) == 0 ? 1 : 0;
		r20 = (r3 | r18) == 0 ? 1 : 0;
		__cost = __cost + (r20 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[5] = r20;
		goto label55;
	label57:
		__cost = __cost + (22);
		r6 = 1325478276 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label58;
		} else {
			goto label59;
		}
	label58:
		__cost = __cost + (76);
		r9 = memory[2];
		r11 = call_v + r9;
		__cost = __cost + (r11 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[2] = r11;
		goto label55;
	label59:
		__cost = __cost + (22);
		r6 = 1748091241 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label60;
		} else {
			goto label65;
		}
	label60:
		__cost = __cost + (10);
		r3 = 0;
		r4 = 0;
		goto label61;
	label61:
		__cost = __cost + (82);
		r8 = memory[0];
		r10 = (addr4 & r8) == 0 ? 1 : 0;
		r12 = r4 < r10 ? 1 : 0;
		r13 = r12 == 0 ? 1 : 0;
		if (r13 != 0) {
			goto label62;
		} else {
			goto label64;
		}
	label62:
		__cost = __cost + (14);
		r2 = r3;
		goto label63;
	label63:
		__cost = __cost + (43);
		r8 = localmem[64];
		// UseMemory operations currently ignored
		localmem[r8] = r2;
		r13 = localmem[64];
		r21 = r8 - r13;
		r23 = 32 + r21;
		// UseMemory operations currently ignored
		goto exit;
	label64:
		__cost = __cost + (84);
		r12 = memory[4];
		r18 = r4 < r12 ? 1 : 0;
		r19 = r18 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (129);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r34 = r4 + addr5;
		r35 = memory[r34];
		r42 = r3 + r35;
		r47 = 1 + r4;
		r3 = r42;
		r4 = r47;
		goto label61;
	label65:
		__cost = __cost + (22);
		r6 = 1905816794 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label66;
		} else {
			goto label67;
		}
	label66:
		__cost = __cost + (68);
		r7 = memory[2];
		r3 = r7;
		goto label51;
	label67:
		__cost = __cost + (22);
		r6 = addr10 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label68;
		} else {
			goto label74;
		}
	label68:
		__cost = __cost + (189);
		localmem[64] = 128;
		// UseMemory operations currently ignored
		localmem[96] = 0;
		r27 = memory[4];
		r30 = 32 * r27;
		r32 = 128 + r30;
		r34 = 32 + r32;
		localmem[64] = r34;
		// UseMemory operations currently ignored
		localmem[128] = r27;
		r63 = r27 == 0 ? 1 : 0;
		r9 = r27;
		if (r63 != 0) {
			goto label69;
		} else {
			goto label71;
		}
	label69:
		__cost = __cost + (16);
		goto label70;
	label70:
		__cost = __cost + (153);
		r21 = localmem[64];
		r25 = 32 + r21;
		r28 = r25 - r21;
		// UseMemory operations currently ignored
		localmem[r21] = r28;
		r33 = localmem[128];
		// UseMemory operations currently ignored
		localmem[r25] = r33;
		r36 = 32 + r25;
		r41 = localmem[128];
		r51 = 32 * r41;
		r64 = 31 + r51;
		r65 = r64 / 32;
		r67 = 15 * r65;
		r69 = 3 + r67;
		// UseMemory operations currently ignored
		// UseMemory operations currently ignored
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (37);
		r74 = r51 + r36;
		r79 = localmem[64];
		r84 = r74 - r79;
		// UseMemory operations currently ignored
		goto exit;
	label71:
		__cost = __cost + (71);
		r12 = 32 * r9;
		r14 = 160 + r12;
		localmem[0] = 4;
		r24 = sha8;
		r7 = r14;
		r8 = r24;
		r9 = 160;
		goto label72;
	label72:
		__cost = __cost + (105);
		r14 = memory[r8];
		// UseMemory operations currently ignored
		localmem[r9] = r14;
		r17 = 32 + r9;
		r22 = 1 + r8;
		r28 = r7 > r17 ? 1 : 0;
		r8 = r22;
		r9 = r17;
		if (r28 != 0) {
			goto label72;
		} else {
			goto label73;
		}
	label73:
		__cost = __cost + (27);
		goto label70;
	label74:
		__cost = __cost + (22);
		r6 = addr11 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label75;
		} else {
			goto label76;
		}
	label75:
		__cost = __cost + (94);
		r11 = calldata4;
		r14 = memory[3];
		r20 = r11 < r14 ? 1 : 0;
		r21 = r20 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (143);
		localmem[0] = 3;
		localmem[0] = addr6;
		localmem[0] = 3;
		r35 = addr6 + r11;
		r36 = memory[r35];
		r42 = (addr2 & r36) == 0 ? 1 : 0;
		r3 = r42;
		goto label48;
	label76:
		__cost = __cost + (22);
		r6 = addr12 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label77;
		} else {
			goto label80;
		}
	label77:
		__cost = __cost + (189);
		localmem[64] = 128;
		// UseMemory operations currently ignored
		localmem[96] = 0;
		r27 = memory[3];
		r30 = 32 * r27;
		r32 = 128 + r30;
		r34 = 32 + r32;
		localmem[64] = r34;
		// UseMemory operations currently ignored
		localmem[128] = r27;
		r63 = r27 == 0 ? 1 : 0;
		r9 = r27;
		if (r63 != 0) {
			goto label69;
		} else {
			goto label78;
		}
	label78:
		__cost = __cost + (71);
		r12 = 32 * r9;
		r14 = 160 + r12;
		localmem[0] = 3;
		r24 = sha9;
		r7 = r14;
		r8 = r24;
		r9 = 160;
		goto label79;
	label79:
		__cost = __cost + (141);
		r15 = memory[r8];
		r21 = (addr2 & r15) == 0 ? 1 : 0;
		// UseMemory operations currently ignored
		localmem[r9] = r21;
		r33 = 1 + r8;
		r38 = 32 + r9;
		r41 = r7 > r38 ? 1 : 0;
		r8 = r33;
		r9 = r38;
		if (r41 != 0) {
			goto label79;
		} else {
			goto label69;
		}
	label80:
		__cost = __cost + (22);
		r6 = addr13 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label81;
		} else {
			goto label121;
		}
	label81:
		__cost = __cost + (109);
		r12 = calldata4;
		r23 = memory[1];
		r24 = r23 + 43200;
		r25 = r24 < timestamp ? 1 : 0;
		r26 = r25 == 0 ? 1 : 0;
		r3 = r12;
		if (r26 != 0) {
			goto label82;
		} else {
			goto label102;
		}
	label82:
		__cost = __cost + (23);
		r10 = call_v < addr0 ? 1 : 0;
		if (r10 != 0) {
			goto label83;
		} else {
			goto label85;
		}
	label83:
		__cost = __cost + (80);
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (30);
		r4 = 0;
		goto label84;
	label84:
		__cost = __cost + (19);
		r2 = r4;
		goto label63;
	label85:
		__cost = __cost + (128);
		__cost = __cost + (timestamp == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[1] = timestamp;
		r23 = memory[3];
		r28 = r23 + 1;
		__cost = __cost + (r28 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[3] = r28;
		r44 = r23 == 0 ? 1 : 0;
		r49 = r44 > r28 ? 1 : 0;
		r7 = r23;
		r10 = r28;
		r11 = r23;
		if (r49 != 0) {
			goto label86;
		} else {
			goto label98;
		}
	label86:
		__cost = __cost + (276);
		localmem[0] = 3;
		r27 = sha10;
		r28 = r27 + r7;
		r30 = memory[r28];
		r37 = (addr1 & r30) == 0 ? 1 : 0;
		r39 = (caller | r37) == 0 ? 1 : 0;
		__cost = __cost + (r39 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r28] = r39;
		r45 = memory[4];
		r48 = r45 + 1;
		__cost = __cost + (r48 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[4] = r48;
		r55 = r45 == 0 ? 1 : 0;
		r60 = r55 > r48 ? 1 : 0;
		r7 = r45;
		r10 = r48;
		r11 = r45;
		if (r60 != 0) {
			goto label87;
		} else {
			goto label94;
		}
	label87:
		__cost = __cost + (243);
		localmem[0] = 4;
		r24 = sha11;
		r28 = call_v * 110;
		r33 = r28 / 100;
		r43 = r7 + r24;
		__cost = __cost + (r33 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r43] = r33;
		r48 = memory[5];
		r54 = (addr2 & r48) == 0 ? 1 : 0;
		r65 = call_v * 5;
		r66 = r65 / 100;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (93);
		r84 = memory[2];
		r85 = r84 < addr3 ? 1 : 0;
		r86 = r85 == 0 ? 1 : 0;
		if (r86 != 0) {
			goto label88;
		} else {
			goto label93;
		}
	label88:
		__cost = __cost + (180);
		r15 = (r3 & addr2) == 0 ? 1 : 0;
		localmem[0] = r15;
		localmem[32] = 6;
		r27 = sha12;
		r28 = memory[r27];
		r33 = r28 < call_v ? 1 : 0;
		if (r33 != 0) {
			goto label89;
		} else {
			goto label92;
		}
	label89:
		__cost = __cost + (430);
		localmem[0] = caller;
		localmem[32] = 6;
		r30 = sha13;
		r32 = memory[r30];
		r36 = call_v * 110;
		r37 = r36 / 100;
		r38 = r37 + r32;
		__cost = __cost + (r38 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r30] = r38;
		r42 = memory[0];
		r44 = memory[2];
		r47 = memory[4];
		r56 = balanceaddress;
		r66 = r56 - r44;
		r77 = (addr4 & r42) == 0 ? 1 : 0;
		r82 = r77 < r47 ? 1 : 0;
		r83 = r82 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (120);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r106 = addr5 + r77;
		r107 = memory[r106];
		r108 = r107 > r66 ? 1 : 0;
		if (r108 != 0) {
			goto label90;
		} else {
			goto label91;
		}
	label90:
		__cost = __cost + (20);
		r4 = 1;
		goto label84;
	label91:
		__cost = __cost + (146);
		r18 = memory[0];
		r21 = memory[3];
		r29 = (addr4 & r18) == 0 ? 1 : 0;
		r34 = r29 < r21 ? 1 : 0;
		r35 = r34 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (376);
		localmem[0] = 3;
		localmem[0] = addr6;
		localmem[0] = 3;
		r53 = addr6 + r29;
		r60 = memory[r53];
		r75 = (addr2 & r60) == 0 ? 1 : 0;
		r81 = (addr2 & r75) == 0 ? 1 : 0;
		r90 = memory[0];
		r101 = (addr4 & r90) == 0 ? 1 : 0;
		r103 = memory[4];
		r105 = r101 < r103 ? 1 : 0;
		r106 = r105 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (130);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r123 = addr5 + r101;
		r125 = memory[r123];
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (163);
		r139 = memory[0];
		r142 = memory[4];
		r159 = (addr4 & r139) == 0 ? 1 : 0;
		r164 = r159 < r142 ? 1 : 0;
		r165 = r164 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (200);
		localmem[0] = 4;
		r174 = memory[3];
		localmem[0] = addr5;
		localmem[0] = 4;
		r187 = r159 + addr5;
		r188 = memory[r187];
		r206 = r159 < r174 ? 1 : 0;
		r207 = r206 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (358);
		localmem[0] = addr6;
		localmem[0] = 4;
		r220 = addr6 + r159;
		r221 = memory[r220];
		r227 = (addr2 & r221) == 0 ? 1 : 0;
		localmem[0] = r227;
		localmem[32] = 6;
		r241 = sha14;
		r243 = memory[r241];
		r253 = r243 - r188;
		__cost = __cost + (r253 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r241] = r253;
		r261 = memory[0];
		r264 = (r261 & addr4) == 0 ? 1 : 0;
		r266 = 1 + r264;
		r278 = (addr7 & r261) == 0 ? 1 : 0;
		r279 = (r278 | r266) == 0 ? 1 : 0;
		__cost = __cost + (r279 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[0] = r279;
		goto label90;
	label92:
		__cost = __cost + (96);
		r25 = (r3 & addr2) == 0 ? 1 : 0;
		r36 = call_v * 5;
		r37 = r36 / 100;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (10);
		goto label89;
	label93:
		__cost = __cost + (81);
		r13 = memory[2];
		r17 = call_v * 5;
		r18 = r17 / 100;
		r19 = r18 + r13;
		__cost = __cost + (r19 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[2] = r19;
		goto label88;
	label94:
		__cost = __cost + (89);
		localmem[0] = 4;
		r20 = sha15;
		r25 = r20 + r11;
		r29 = r20 + r10;
		r12 = r25;
		r13 = r29;
		goto label95;
	label95:
		__cost = __cost + (26);
		r18 = r12 > r13 ? 1 : 0;
		r19 = r18 == 0 ? 1 : 0;
		if (r19 != 0) {
			goto label96;
		} else {
			goto label97;
		}
	label96:
		__cost = __cost + (14);
		goto label87;
	label97:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r13] = 0;
		r19 = 1 + r13;
		r13 = r19;
		goto label95;
	label98:
		__cost = __cost + (89);
		localmem[0] = 3;
		r20 = sha16;
		r25 = r20 + r11;
		r29 = r20 + r10;
		r12 = r25;
		r13 = r29;
		goto label99;
	label99:
		__cost = __cost + (26);
		r18 = r12 > r13 ? 1 : 0;
		r19 = r18 == 0 ? 1 : 0;
		if (r19 != 0) {
			goto label100;
		} else {
			goto label101;
		}
	label100:
		__cost = __cost + (14);
		goto label86;
	label101:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r13] = 0;
		r19 = 1 + r13;
		r13 = r19;
		goto label99;
	label102:
		__cost = __cost + (79);
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (112);
		r47 = memory[3];
		r57 = r47 + -1;
		r62 = r57 < r47 ? 1 : 0;
		r63 = r62 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (253);
		r69 = memory[3];
		localmem[0] = 3;
		r75 = memory[2];
		r85 = r69 + addr8;
		r86 = memory[r85];
		r92 = (addr2 & r86) == 0 ? 1 : 0;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (152);
		r112 = memory[5];
		r122 = (addr2 & r112) == 0 ? 1 : 0;
		r128 = balanceaddress;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (120);
		r141 = memory[0];
		r144 = (addr7 & r141) == 0 ? 1 : 0;
		__cost = __cost + (r144 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[0] = r144;
		__cost = __cost + (timestamp == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[1] = timestamp;
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[2] = 0;
		goto label120;
	label103:
		__cost = __cost + (198);
		// UseMemory operations currently ignored
		localmem[96] = 0;
		localmem[64] = 128;
		r31 = memory[3];
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[3] = 0;
		localmem[0] = 3;
		localmem[0] = addr6;
		localmem[0] = 3;
		r62 = addr6 + r31;
		r7 = r62;
		r9 = addr6;
		goto label104;
	label104:
		__cost = __cost + (14);
		r17 = r7;
		r8 = r7;
		goto label105;
	label105:
		__cost = __cost + (26);
		r14 = r8 > r9 ? 1 : 0;
		r15 = r14 == 0 ? 1 : 0;
		if (r15 != 0) {
			goto label106;
		} else {
			goto label116;
		}
	label106:
		__cost = __cost + (49);
		goto label115;
	label107:
		__cost = __cost + (198);
		// UseMemory operations currently ignored
		localmem[128] = 0;
		localmem[64] = 160;
		r31 = memory[4];
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[4] = 0;
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r62 = addr5 + r31;
		r7 = r62;
		r9 = addr5;
		goto label108;
	label108:
		__cost = __cost + (14);
		r17 = r7;
		r8 = r7;
		goto label109;
	label109:
		__cost = __cost + (26);
		r14 = r8 > r9 ? 1 : 0;
		r15 = r14 == 0 ? 1 : 0;
		if (r15 != 0) {
			goto label110;
		} else {
			goto label111;
		}
	label110:
		__cost = __cost + (119);
		r16 = memory[7];
		r20 = (r16 & -256) == 0 ? 1 : 0;
		r28 = (r16 & 255) == 0 ? 1 : 0;
		r30 = 1 + r28;
		r31 = (r30 | r20) == 0 ? 1 : 0;
		__cost = __cost + (r31 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[7] = r31;
		r4 = 0;
		goto label84;
	label111:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r9] = 0;
		r15 = 1 + r9;
		r9 = r15;
		goto label109;
	label112:
		goto exit;
	label113:
		goto exit;
	label114:
		goto exit;
	label115:
		__cost = __cost + (4);
		goto label107;
	label116:
		__cost = __cost + (111);
		r15 = memory[r9];
		r22 = (addr1 & r15) == 0 ? 1 : 0;
		__cost = __cost + (r22 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r9] = r22;
		r25 = 1 + r9;
		r9 = r25;
		goto label105;
	label117:
		goto exit;
	label118:
		goto exit;
	label119:
		goto exit;
	label120:
		__cost = __cost + (4);
		goto label103;
	label121:
		__cost = __cost + (22);
		r6 = addr14 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label122;
		} else {
			goto label123;
		}
	label122:
		__cost = __cost + (94);
		r11 = calldata4;
		r14 = memory[4];
		r20 = r11 < r14 ? 1 : 0;
		r21 = r20 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (108);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r35 = addr5 + r11;
		r36 = memory[r35];
		r3 = r36;
		goto label51;
	label123:
		__cost = __cost + (22);
		r6 = addr15 == r1 ? 1 : 0;
		if (r6 != 0) {
			goto label124;
		} else {
			goto label127;
		}
	label124:
		__cost = __cost + (66);
		r9 = memory[0];
		r11 = (addr4 & r9) == 0 ? 1 : 0;
		r3 = 0;
		r4 = r11;
		goto label125;
	label125:
		__cost = __cost + (76);
		r8 = memory[4];
		r10 = r4 < r8 ? 1 : 0;
		r11 = r10 == 0 ? 1 : 0;
		if (r11 != 0) {
			goto label62;
		} else {
			goto label126;
		}
	label126:
		__cost = __cost + (84);
		r12 = memory[4];
		r18 = r4 < r12 ? 1 : 0;
		r19 = r18 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (129);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r34 = r4 + addr5;
		r35 = memory[r34];
		r42 = r3 + r35;
		r47 = 1 + r4;
		r3 = r42;
		r4 = r47;
		goto label125;
	label127:
		__cost = __cost + (120);
		r25 = memory[1];
		r26 = r25 + 43200;
		r27 = r26 < timestamp ? 1 : 0;
		r28 = r27 == 0 ? 1 : 0;
		if (r28 != 0) {
			goto label128;
		} else {
			goto label148;
		}
	label128:
		__cost = __cost + (23);
		r11 = call_v < addr0 ? 1 : 0;
		if (r11 != 0) {
			goto label129;
		} else {
			goto label131;
		}
	label129:
		__cost = __cost + (80);
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (30);
		goto label130;
	label130:
		__cost = __cost + (19);
		goto label54;
	label131:
		__cost = __cost + (128);
		__cost = __cost + (timestamp == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[1] = timestamp;
		r24 = memory[3];
		r29 = r24 + 1;
		__cost = __cost + (r29 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[3] = r29;
		r45 = r24 == 0 ? 1 : 0;
		r50 = r45 > r29 ? 1 : 0;
		r8 = r24;
		r11 = r29;
		r12 = r24;
		if (r50 != 0) {
			goto label132;
		} else {
			goto label144;
		}
	label132:
		__cost = __cost + (276);
		localmem[0] = 3;
		r28 = sha17;
		r29 = r28 + r8;
		r31 = memory[r29];
		r38 = (addr1 & r31) == 0 ? 1 : 0;
		r40 = (caller | r38) == 0 ? 1 : 0;
		__cost = __cost + (r40 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r29] = r40;
		r46 = memory[4];
		r49 = r46 + 1;
		__cost = __cost + (r49 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[4] = r49;
		r56 = r46 == 0 ? 1 : 0;
		r61 = r56 > r49 ? 1 : 0;
		r8 = r46;
		r11 = r49;
		r12 = r46;
		if (r61 != 0) {
			goto label133;
		} else {
			goto label140;
		}
	label133:
		__cost = __cost + (243);
		localmem[0] = 4;
		r25 = sha18;
		r29 = call_v * 110;
		r34 = r29 / 100;
		r44 = r8 + r25;
		__cost = __cost + (r34 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r44] = r34;
		r49 = memory[5];
		r55 = (addr2 & r49) == 0 ? 1 : 0;
		r66 = call_v * 5;
		r67 = r66 / 100;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (93);
		r85 = memory[2];
		r86 = r85 < addr3 ? 1 : 0;
		r87 = r86 == 0 ? 1 : 0;
		if (r87 != 0) {
			goto label134;
		} else {
			goto label139;
		}
	label134:
		__cost = __cost + (180);
		localmem[0] = 0;
		localmem[32] = 6;
		r28 = sha19;
		r29 = memory[r28];
		r34 = r29 < call_v ? 1 : 0;
		if (r34 != 0) {
			goto label135;
		} else {
			goto label138;
		}
	label135:
		__cost = __cost + (430);
		localmem[0] = caller;
		localmem[32] = 6;
		r31 = sha20;
		r33 = memory[r31];
		r37 = call_v * 110;
		r38 = r37 / 100;
		r39 = r38 + r33;
		__cost = __cost + (r39 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r31] = r39;
		r43 = memory[0];
		r45 = memory[2];
		r48 = memory[4];
		r57 = balanceaddress;
		r67 = r57 - r45;
		r78 = (addr4 & r43) == 0 ? 1 : 0;
		r83 = r78 < r48 ? 1 : 0;
		r84 = r83 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (120);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r107 = addr5 + r78;
		r108 = memory[r107];
		r109 = r108 > r67 ? 1 : 0;
		if (r109 != 0) {
			goto label136;
		} else {
			goto label137;
		}
	label136:
		__cost = __cost + (20);
		goto label130;
	label137:
		__cost = __cost + (146);
		r19 = memory[0];
		r22 = memory[3];
		r30 = (addr4 & r19) == 0 ? 1 : 0;
		r35 = r30 < r22 ? 1 : 0;
		r36 = r35 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (376);
		localmem[0] = 3;
		localmem[0] = addr6;
		localmem[0] = 3;
		r54 = addr6 + r30;
		r61 = memory[r54];
		r76 = (addr2 & r61) == 0 ? 1 : 0;
		r82 = (addr2 & r76) == 0 ? 1 : 0;
		r91 = memory[0];
		r102 = (addr4 & r91) == 0 ? 1 : 0;
		r104 = memory[4];
		r106 = r102 < r104 ? 1 : 0;
		r107 = r106 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (130);
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r124 = addr5 + r102;
		r126 = memory[r124];
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (163);
		r140 = memory[0];
		r143 = memory[4];
		r160 = (addr4 & r140) == 0 ? 1 : 0;
		r165 = r160 < r143 ? 1 : 0;
		r166 = r165 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (200);
		localmem[0] = 4;
		r175 = memory[3];
		localmem[0] = addr5;
		localmem[0] = 4;
		r188 = r160 + addr5;
		r189 = memory[r188];
		r207 = r160 < r175 ? 1 : 0;
		r208 = r207 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (358);
		localmem[0] = addr6;
		localmem[0] = 4;
		r221 = addr6 + r160;
		r222 = memory[r221];
		r228 = (addr2 & r222) == 0 ? 1 : 0;
		localmem[0] = r228;
		localmem[32] = 6;
		r242 = sha21;
		r244 = memory[r242];
		r254 = r244 - r189;
		__cost = __cost + (r254 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r242] = r254;
		r262 = memory[0];
		r265 = (r262 & addr4) == 0 ? 1 : 0;
		r267 = 1 + r265;
		r279 = (addr7 & r262) == 0 ? 1 : 0;
		r280 = (r279 | r267) == 0 ? 1 : 0;
		__cost = __cost + (r280 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[0] = r280;
		goto label136;
	label138:
		__cost = __cost + (96);
		r37 = call_v * 5;
		r38 = r37 / 100;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (10);
		goto label135;
	label139:
		__cost = __cost + (81);
		r14 = memory[2];
		r18 = call_v * 5;
		r19 = r18 / 100;
		r20 = r19 + r14;
		__cost = __cost + (r20 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[2] = r20;
		goto label134;
	label140:
		__cost = __cost + (89);
		localmem[0] = 4;
		r21 = sha22;
		r26 = r21 + r12;
		r30 = r21 + r11;
		r13 = r26;
		r14 = r30;
		goto label141;
	label141:
		__cost = __cost + (26);
		r19 = r13 > r14 ? 1 : 0;
		r20 = r19 == 0 ? 1 : 0;
		if (r20 != 0) {
			goto label142;
		} else {
			goto label143;
		}
	label142:
		__cost = __cost + (14);
		goto label133;
	label143:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r14] = 0;
		r20 = 1 + r14;
		r14 = r20;
		goto label141;
	label144:
		__cost = __cost + (89);
		localmem[0] = 3;
		r21 = sha23;
		r26 = r21 + r12;
		r30 = r21 + r11;
		r13 = r26;
		r14 = r30;
		goto label145;
	label145:
		__cost = __cost + (26);
		r19 = r13 > r14 ? 1 : 0;
		r20 = r19 == 0 ? 1 : 0;
		if (r20 != 0) {
			goto label146;
		} else {
			goto label147;
		}
	label146:
		__cost = __cost + (14);
		goto label132;
	label147:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r14] = 0;
		r20 = 1 + r14;
		r14 = r20;
		goto label145;
	label148:
		__cost = __cost + (79);
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (112);
		r48 = memory[3];
		r58 = r48 + -1;
		r63 = r58 < r48 ? 1 : 0;
		r64 = r63 == 0 ? 1 : 0;
		// IThrow ignored
		__cost = __cost + (253);
		r70 = memory[3];
		localmem[0] = 3;
		r76 = memory[2];
		r86 = r70 + addr8;
		r87 = memory[r86];
		r93 = (addr2 & r87) == 0 ? 1 : 0;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (152);
		r113 = memory[5];
		r123 = (addr2 & r113) == 0 ? 1 : 0;
		r129 = balanceaddress;
		__cost = __cost + (0);
		// Call ignored
		__cost = __cost + (120);
		r142 = memory[0];
		r145 = (addr7 & r142) == 0 ? 1 : 0;
		__cost = __cost + (r145 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[0] = r145;
		__cost = __cost + (timestamp == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[1] = timestamp;
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[2] = 0;
		goto label166;
	label149:
		__cost = __cost + (198);
		// UseMemory operations currently ignored
		localmem[96] = 0;
		localmem[64] = 128;
		r32 = memory[3];
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[3] = 0;
		localmem[0] = 3;
		localmem[0] = addr6;
		localmem[0] = 3;
		r63 = addr6 + r32;
		r8 = r63;
		r10 = addr6;
		goto label150;
	label150:
		__cost = __cost + (14);
		r18 = r8;
		r9 = r8;
		goto label151;
	label151:
		__cost = __cost + (26);
		r15 = r9 > r10 ? 1 : 0;
		r16 = r15 == 0 ? 1 : 0;
		if (r16 != 0) {
			goto label152;
		} else {
			goto label162;
		}
	label152:
		__cost = __cost + (49);
		goto label161;
	label153:
		__cost = __cost + (198);
		// UseMemory operations currently ignored
		localmem[128] = 0;
		localmem[64] = 160;
		r32 = memory[4];
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[4] = 0;
		localmem[0] = 4;
		localmem[0] = addr5;
		localmem[0] = 4;
		r63 = addr5 + r32;
		r8 = r63;
		r10 = addr5;
		goto label154;
	label154:
		__cost = __cost + (14);
		r18 = r8;
		r9 = r8;
		goto label155;
	label155:
		__cost = __cost + (26);
		r15 = r9 > r10 ? 1 : 0;
		r16 = r15 == 0 ? 1 : 0;
		if (r16 != 0) {
			goto label156;
		} else {
			goto label157;
		}
	label156:
		__cost = __cost + (119);
		r17 = memory[7];
		r21 = (r17 & -256) == 0 ? 1 : 0;
		r29 = (r17 & 255) == 0 ? 1 : 0;
		r31 = 1 + r29;
		r32 = (r31 | r21) == 0 ? 1 : 0;
		__cost = __cost + (r32 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[7] = r32;
		goto label130;
	label157:
		__cost = __cost + (23);
		__cost = __cost + (0 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r10] = 0;
		r16 = 1 + r10;
		r10 = r16;
		goto label155;
	label158:
		goto exit;
	label159:
		goto exit;
	label160:
		goto exit;
	label161:
		__cost = __cost + (4);
		goto label153;
	label162:
		__cost = __cost + (111);
		r16 = memory[r10];
		r23 = (addr1 & r16) == 0 ? 1 : 0;
		__cost = __cost + (r23 == 0 ? 5000 : 20000);
		// Refund __cost ignored
		memory[r10] = r23;
		r26 = 1 + r10;
		r10 = r26;
		goto label151;
	label163:
		goto exit;
	label164:
		goto exit;
	label165:
		goto exit;
	label166:
		__cost = __cost + (4);
		goto label149;
        exit:
		__VERIFIER_print_hull(__cost);
		return 0;

}
