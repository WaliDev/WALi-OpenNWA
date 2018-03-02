public class TransformedHelloWorld {
        static int gas = 0;
	public static int entryPoint(int call_v, int[] memory) {
		gas = 0;
		int[] localmem = new int[1000];
		int r6;
		r6 = 0;
		//case 1:
		gas = gas + (27);
		// UseMemory operations currently ignored
		localmem[64] = 96;
		r6 = call_v == 0 ? 1 : 0;
		if (r6 != 0) {
			//case 2:
			gas = gas + (52);
			// UseMemory operations currently ignored
			localmem[0] = 0;
			return 0;
		} else {
			//case 3:
			gas = gas + (6);
			return 0;
		}
	}

	public static void main(int k) {
		entryPoint(k, new int [10]);	
		return;
	}
}
