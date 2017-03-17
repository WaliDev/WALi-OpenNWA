#define DECLARE_POW(BASE) int icra_pow ## BASE (int y) { int z = 1; int i; for(i = 0; i < y; i++) { z = z * BASE; } return z; }

int icra_pow(int x, int y) {
    int z = 1;
    int i;
    for(i = 0; i < y; i++) {
        z = x * z;
    }
    return z;
}
