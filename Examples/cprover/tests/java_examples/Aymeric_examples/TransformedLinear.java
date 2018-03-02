//import java.io.FileInputStream;
//import java.io.IOException;
//import java.lang.Math;
//import java.nio.ByteBuffer;

//import gov.nasa.jpf.symbc.Debug;

public class TransformedLinear {
	static int gas = 0;
	public static int entryPoint(int addr0, int addr1, int addr2, int call_dsize, int call_v, int calldata0, int calldata4, int[] memory) {
		gas = 0;
		int[] localmem = new int[1000];
		int r10, r11, r12, r15, r3, r5, r7;
		r10 = r11 = r12 = r15 = r3 = r5 = r7 = 0;
		//case 1:
			gas = gas + (30);
			// UseMemory operations currently ignored
			localmem[64] = 96;
			r7 = call_dsize < 4 ? 1 : 0;
			if (r7 != 0) {
				//case 2:
				gas = gas + (7);
				return 0;
	
			} else {
				//case 3:
				gas = gas + (45);
				r5 = calldata0;
				r10 = r5 / addr0;
				r12 = (addr1 & r10) == 0 ? 1 : 0;
				r15 = addr2 == r12 ? 1 : 0;
				if (r15 != 0) {
					//case 4:
					gas = gas + (19);
					r5 = call_v == 0 ? 1 : 0;
					if (r5 != 0) {
						//case 5:
						gas = gas + (64);
						r12 = calldata4;
						r3 = r12;
						r5 = 0;
						//case 6:
						while (true){
							gas = gas + (26);
							r10 = r5 < r3 ? 1 : 0;
							r11 = r10 == 0 ? 1 : 0;
							if (r11 != 0) {
								//case 7:
								gas = gas + (77);
								// UseMemory operations currently ignored
								localmem[96] = r5;
								return 0;
							} else {
								//case 8:
								gas = gas + (25);
								r10 = 1 + r5;
								r5 = r10;
							}
						}
					} else {
						//case 9:
						gas = gas + (6);
						return 0;
					}

				} else {
					//case 10:
					gas = gas + (7);
					return 0;
				}

               		} 
	}

	public static void main(String[] args) {
		entryPoint(0, 0, 0, 0, 0, 0, 0,  new int[10]);
		return;
	}
}
