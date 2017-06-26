int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1, int K1, int L1, int M1, int N1, int O1, int P1, int Q1, int R1, int S1, int T1, int U1, int V1, int W1, int X1, int Y1, int Z1, int A2, int B2, int C2) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    if (1 >= 0) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f49;
    }
   }
  goto end;
 }
 loc_f102:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = -1 + J;
    if (J >= 0) {
     J = J_;
     goto loc_f102;
    }
   }
   if (nondet_bool()) {
    int Z1_ = nondet();
    int K_ = nondet();
    if (0 >= 1 + J) {
     K = K_;
     Z1 = Z1_;
     goto loc_f114;
    }
   }
  goto end;
 }
 loc_f114:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = -1 + K;
    if (K >= 0) {
     K = K_;
     goto loc_f114;
    }
   }
   if (nondet_bool()) {
    int L_ = nondet();
    if (0 >= 1 + K) {
     L = L_;
     goto loc_f124;
    }
   }
  goto end;
 }
 loc_f124:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = nondet();
    if (H >= 1) {
     M = M_;
     goto loc_f127;
    }
   }
   if (nondet_bool()) {
    if (0 >= H) {
     goto loc_f138;
    }
   }
  goto end;
 }
 loc_f127:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = -1 + M;
    if (M >= 0) {
     M = M_;
     goto loc_f127;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + M) {
     goto loc_f138;
    }
   }
  goto end;
 }
 loc_f138:
 {
 __cost++;
   if (nondet_bool()) {
    if (11 >= B) {
     goto loc_f140;
    }
   }
   if (nondet_bool()) {
    if (B >= 13) {
     goto loc_f140;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int O_ = 0;
    int B_ = 12;
    if (B == 12) {
     B = B_;
     N = N_;
     O = O_;
     goto loc_f152;
    }
   }
  goto end;
 }
 loc_f140:
 {
 __cost++;
   if (nondet_bool()) {
    if (14 >= B) {
     goto loc_f142;
    }
   }
   if (nondet_bool()) {
    if (B >= 16) {
     goto loc_f142;
    }
   }
   if (nondet_bool()) {
    int T_ = nondet();
    int B_ = 15;
    if (B == 15) {
     B = B_;
     T = T_;
     goto loc_f193;
    }
   }
  goto end;
 }
 loc_f142:
 {
 __cost++;
   if (nondet_bool()) {
    if (122 >= B) {
     goto loc_f144;
    }
   }
   if (nondet_bool()) {
    if (B >= 124) {
     goto loc_f144;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    int B_ = 123;
    if (B == 123) {
     B = B_;
     X = X_;
     goto loc_f239;
    }
   }
  goto end;
 }
 loc_f144:
 {
 __cost++;
   if (nondet_bool()) {
    if (19845 >= B) {
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    if (B >= 19847) {
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    int H1_ = nondet();
    int B_ = 19846;
    if (B == 19846) {
     B = B_;
     H1 = H1_;
     goto loc_f294;
    }
   }
  goto end;
 }
 loc_f152:
 {
 __cost++;
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = -1 + N;
    if (N >= 0) {
     N = N_;
     O = O_;
     goto loc_f152;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int Q_ = 0;
    if (0 >= 1 + N) {
     P = P_;
     Q = Q_;
     goto loc_f165;
    }
   }
  goto end;
 }
 loc_f165:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = -1 + P;
    if (P >= 0) {
     P = P_;
     Q = Q_;
     goto loc_f165;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int S_ = 0;
    if (0 >= 1 + P) {
     R = R_;
     S = S_;
     goto loc_f178;
    }
   }
  goto end;
 }
 loc_f178:
 {
 __cost++;
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = -1 + R;
    if (R >= 0) {
     R = R_;
     S = S_;
     goto loc_f178;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + R) {
     goto loc_f135;
    }
   }
  goto end;
 }
 loc_f193:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = -1 + T;
    if (T >= 0) {
     T = T_;
     goto loc_f193;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    if (0 >= 1 + T) {
     U = U_;
     goto loc_f202;
    }
   }
  goto end;
 }
 loc_f202:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = -1 + U;
    if (U >= 0) {
     U = U_;
     goto loc_f202;
    }
   }
   if (nondet_bool()) {
    int U_ = nondet();
    if (0 >= 1 + U) {
     U = U_;
     goto loc_f208;
    }
   }
  goto end;
 }
 loc_f208:
 {
 __cost++;
   if (nondet_bool()) {
    int U_ = -1 + U;
    if (U >= 0) {
     U = U_;
     goto loc_f208;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int W_ = 0;
    if (0 >= 1 + U) {
     V = V_;
     W = W_;
     goto loc_f223;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int W_ = 0;
    if (U >= 0) {
     V = V_;
     W = W_;
     goto loc_f223;
    }
   }
  goto end;
 }
 loc_f223:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = nondet();
    int V_ = -1 + V;
    if (V >= 0) {
     V = V_;
     W = W_;
     goto loc_f223;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + V) {
     goto loc_f135;
    }
   }
  goto end;
 }
 loc_f239:
 {
 __cost++;
   if (nondet_bool()) {
    int X_ = -1 + X;
    if (X >= 0) {
     X = X_;
     goto loc_f239;
    }
   }
   if (nondet_bool()) {
    int Y_ = nondet();
    if (0 >= 1 + X) {
     Y = Y_;
     goto loc_f247;
    }
   }
  goto end;
 }
 loc_f247:
 {
 __cost++;
   if (nondet_bool()) {
    int A1_ = nondet();
    int Z_ = nondet();
    if (Y >= 8) {
     Z = Z_;
     A1 = A1_;
     goto loc_f252;
    }
   }
   if (nondet_bool()) {
    if (7 >= Y) {
     goto loc_f135;
    }
   }
  goto end;
 }
 loc_f252:
 {
 __cost++;
   if (nondet_bool()) {
    int A1_ = -1 + A1;
    if (A1 >= 0) {
     A1 = A1_;
     goto loc_f252;
    }
   }
   if (nondet_bool()) {
    int B1_ = nondet();
    if (0 >= 1 + A1) {
     B1 = B1_;
     goto loc_f260;
    }
   }
  goto end;
 }
 loc_f260:
 {
 __cost++;
   if (nondet_bool()) {
    int E1_ = nondet();
    int D1_ = nondet();
    int C1_ = nondet();
    int B1_ = -1 + B1;
    if (B1 >= 8) {
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     goto loc_f260;
    }
   }
   if (nondet_bool()) {
    int F1_ = nondet();
    int G1_ = 0;
    if (7 >= B1) {
     F1 = F1_;
     G1 = G1_;
     goto loc_f274;
    }
   }
  goto end;
 }
 loc_f274:
 {
 __cost++;
   if (nondet_bool()) {
    int G1_ = nondet();
    int F1_ = -1 + F1;
    if (F1 >= 0) {
     F1 = F1_;
     G1 = G1_;
     goto loc_f274;
    }
   }
   if (nondet_bool()) {
    int Y_ = -1 + Y;
    if (0 >= 1 + F1) {
     Y = Y_;
     goto loc_f247;
    }
   }
  goto end;
 }
 loc_f294:
 {
 __cost++;
   if (nondet_bool()) {
    int H1_ = -1 + H1;
    if (H1 >= 0) {
     H1 = H1_;
     goto loc_f294;
    }
   }
   if (nondet_bool()) {
    int I1_ = nondet();
    if (0 >= 1 + H1) {
     I1 = I1_;
     goto loc_f304;
    }
   }
  goto end;
 }
 loc_f304:
 {
 __cost++;
   if (nondet_bool()) {
    int I1_ = -1 + I1;
    if (I1 >= 0) {
     I1 = I1_;
     goto loc_f304;
    }
   }
   if (nondet_bool()) {
    int K1_ = nondet();
    int J1_ = 0;
    if (0 >= 1 + I1) {
     J1 = J1_;
     K1 = K1_;
     goto loc_f313;
    }
   }
  goto end;
 }
 loc_f313:
 {
 __cost++;
   if (nondet_bool()) {
    int K1_ = nondet();
    int J1_ = 1 + J1;
    if (15 >= J1) {
     J1 = J1_;
     K1 = K1_;
     goto loc_f313;
    }
   }
   if (nondet_bool()) {
    int M1_ = nondet();
    int L1_ = 0;
    if (J1 >= 16) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f322;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int M1_ = nondet();
    int L1_ = 0;
    if (0 >= 1 + Y_0 && 15 >= J1) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f322;
    }
   }
   if (nondet_bool()) {
    int M1_ = nondet();
    int L1_ = 0;
    if (15 >= J1) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f322;
    }
   }
  goto end;
 }
 loc_f322:
 {
 __cost++;
   if (nondet_bool()) {
    int M1_ = nondet();
    int L1_ = 1 + L1;
    int K1_ = -1 + K1;
    if (15 >= L1) {
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     goto loc_f322;
    }
   }
   if (nondet_bool()) {
    int N1_ = nondet();
    int J1_ = 0;
    if (L1 >= 16) {
     J1 = J1_;
     N1 = N1_;
     goto loc_f332;
    }
   }
   if (nondet_bool()) {
    int N1_ = nondet();
    int J1_ = 0;
    if (15 >= L1) {
     J1 = J1_;
     N1 = N1_;
     goto loc_f332;
    }
   }
  goto end;
 }
 loc_f332:
 {
 __cost++;
   if (nondet_bool()) {
    int N1_ = nondet();
    int J1_ = 1 + J1;
    if (15 >= J1) {
     J1 = J1_;
     N1 = N1_;
     goto loc_f332;
    }
   }
   if (nondet_bool()) {
    int M1_ = nondet();
    int L1_ = 0;
    if (J1 >= 16) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f341;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int M1_ = nondet();
    int L1_ = 0;
    if (0 >= 1 + Y_0 && 15 >= J1) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f341;
    }
   }
   if (nondet_bool()) {
    int M1_ = nondet();
    int L1_ = 0;
    if (15 >= J1) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f341;
    }
   }
  goto end;
 }
 loc_f341:
 {
 __cost++;
   if (nondet_bool()) {
    if (L1 >= 16) {
     goto loc_f349;
    }
   }
   if (nondet_bool()) {
    int M1_ = nondet();
    int N1_ = -1 + N1;
    int L1_ = 1 + L1;
    if (15 >= L1) {
     L1 = L1_;
     M1 = M1_;
     N1 = N1_;
     goto loc_f341;
    }
   }
   if (nondet_bool()) {
    if (15 >= L1) {
     goto loc_f349;
    }
   }
  goto end;
 }
 loc_f349:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + N1) {
     goto loc_f368;
    }
   }
   if (nondet_bool()) {
    if (N1 >= 1) {
     goto loc_f368;
    }
   }
   if (nondet_bool()) {
    int O1_ = nondet();
    int N1_ = 0;
    if (N1 == 0) {
     N1 = N1_;
     O1 = O1_;
     goto loc_f353;
    }
   }
  goto end;
 }
 loc_f353:
 {
 __cost++;
   if (nondet_bool()) {
    int O1_ = -1 + O1;
    if (O1 >= 0) {
     O1 = O1_;
     goto loc_f353;
    }
   }
   if (nondet_bool()) {
    int P1_ = nondet();
    if (0 >= 1 + O1) {
     P1 = P1_;
     goto loc_f362;
    }
   }
  goto end;
 }
 loc_f362:
 {
 __cost++;
   if (nondet_bool()) {
    int P1_ = -1 + P1;
    if (P1 >= 0) {
     P1 = P1_;
     goto loc_f362;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + P1) {
     goto loc_f368;
    }
   }
  goto end;
 }
 loc_f368:
 {
 __cost++;
   if (nondet_bool()) {
    int Q1_ = nondet();
    if (1 >= 0) {
     Q1 = Q1_;
     goto loc_f372;
    }
   }
   if (nondet_bool()) {
    int S1_ = K1 + -N1;
    int J1_ = K1 + -N1;
    if (1 >= 0) {
     J1 = J1_;
     S1 = S1_;
     goto loc_f391;
    }
   }
  goto end;
 }
 loc_f372:
 {
 __cost++;
   if (nondet_bool()) {
    int Q1_ = -1 + Q1;
    if (Q1 >= 0) {
     Q1 = Q1_;
     goto loc_f372;
    }
   }
   if (nondet_bool()) {
    int R1_ = nondet();
    if (0 >= 1 + Q1) {
     R1 = R1_;
     goto loc_f382;
    }
   }
  goto end;
 }
 loc_f382:
 {
 __cost++;
   if (nondet_bool()) {
    int R1_ = -1 + R1;
    if (R1 >= 0) {
     R1 = R1_;
     goto loc_f382;
    }
   }
   if (nondet_bool()) {
    int S1_ = K1 + -N1;
    int J1_ = K1 + -N1;
    if (0 >= 1 + R1) {
     J1 = J1_;
     S1 = S1_;
     goto loc_f391;
    }
   }
  goto end;
 }
 loc_f391:
 {
 __cost++;
   if (nondet_bool()) {
    int L1_ = 0;
    if (J1 >= 17) {
     L1 = L1_;
     goto loc_f394;
    }
   }
   if (nondet_bool()) {
    int M1_ = 0;
    if (16 >= J1) {
     M1 = M1_;
     goto loc_f403;
    }
   }
  goto end;
 }
 loc_f394:
 {
 __cost++;
   if (nondet_bool()) {
    int L1_ = 1 + L1;
    if (14 >= L1) {
     L1 = L1_;
     goto loc_f394;
    }
   }
   if (nondet_bool()) {
    int J1_ = nondet();
    if (L1 >= 15) {
     J1 = J1_;
     goto loc_f391;
    }
   }
  goto end;
 }
 loc_f403:
 {
 __cost++;
   if (nondet_bool()) {
    int L1_ = nondet();
    if (J1 >= 1) {
     L1 = L1_;
     goto loc_f406;
    }
   }
   if (nondet_bool()) {
    int T1_ = nondet();
    if (0 >= J1) {
     T1 = T1_;
     goto loc_f419;
    }
   }
  goto end;
 }
 loc_f406:
 {
 __cost++;
   if (nondet_bool()) {
    int M1_ = 1;
    int L1_ = -1 + L1;
    if (L1 >= 0) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f406;
    }
   }
   if (nondet_bool()) {
    int M1_ = 0;
    int L1_ = -1 + L1;
    if (L1 >= 0) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f406;
    }
   }
   if (nondet_bool()) {
    int J1_ = -1 + J1;
    if (0 >= 1 + L1) {
     J1 = J1_;
     goto loc_f403;
    }
   }
  goto end;
 }
 loc_f419:
 {
 __cost++;
   if (nondet_bool()) {
    int T1_ = -1 + T1;
    if (T1 >= 0) {
     T1 = T1_;
     goto loc_f419;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + T1) {
     goto loc_f426;
    }
   }
  goto end;
 }
 loc_f426:
 {
 __cost++;
   if (nondet_bool()) {
    int L1_ = nondet();
    int J1_ = nondet();
    if (S1 >= 0) {
     J1 = J1_;
     L1 = L1_;
     goto loc_f430;
    }
   }
   if (nondet_bool()) {
    int Y1_ = nondet();
    if (0 >= 1 + S1) {
     Y1 = Y1_;
     goto loc_f502;
    }
   }
  goto end;
 }
 loc_f430:
 {
 __cost++;
   if (nondet_bool()) {
    int J1_ = 1 + J1;
    if (14 >= J1) {
     J1 = J1_;
     goto loc_f430;
    }
   }
   if (nondet_bool()) {
    if (J1 >= 15) {
     goto loc_f436;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    if (Y_0 >= 1 + Y_1) {
     goto loc_f436;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f436;
    }
   }
  goto end;
 }
 loc_f436:
 {
 __cost++;
   if (nondet_bool()) {
    int U1_ = nondet();
    if (1 >= 0) {
     U1 = U1_;
     goto loc_f443;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int L1_ = nondet();
    if (Y_0 >= 1 + Y_1) {
     L1 = L1_;
     goto loc_f486;
    }
   }
  goto end;
 }
 loc_f443:
 {
 __cost++;
   if (nondet_bool()) {
    int U1_ = -1 + U1;
    if (U1 >= 0) {
     U1 = U1_;
     goto loc_f443;
    }
   }
   if (nondet_bool()) {
    int V1_ = nondet();
    if (0 >= 1 + U1) {
     V1 = V1_;
     goto loc_f452;
    }
   }
  goto end;
 }
 loc_f452:
 {
 __cost++;
   if (nondet_bool()) {
    int V1_ = -1 + V1;
    if (V1 >= 0) {
     V1 = V1_;
     goto loc_f452;
    }
   }
   if (nondet_bool()) {
    int V1_ = nondet();
    if (0 >= 1 + V1) {
     V1 = V1_;
     goto loc_f458;
    }
   }
  goto end;
 }
 loc_f458:
 {
 __cost++;
   if (nondet_bool()) {
    int V1_ = -1 + V1;
    if (V1 >= 0) {
     V1 = V1_;
     goto loc_f458;
    }
   }
   if (nondet_bool()) {
    int W1_ = nondet();
    int X1_ = 0;
    if (0 >= 1 + V1) {
     W1 = W1_;
     X1 = X1_;
     goto loc_f473;
    }
   }
   if (nondet_bool()) {
    int W1_ = nondet();
    int X1_ = 0;
    if (V1 >= 0) {
     W1 = W1_;
     X1 = X1_;
     goto loc_f473;
    }
   }
  goto end;
 }
 loc_f473:
 {
 __cost++;
   if (nondet_bool()) {
    int X1_ = nondet();
    int W1_ = -1 + W1;
    if (W1 >= 0) {
     W1 = W1_;
     X1 = X1_;
     goto loc_f473;
    }
   }
   if (nondet_bool()) {
    int M1_ = nondet();
    int L1_ = nondet();
    if (0 >= 1 + W1) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f486;
    }
   }
  goto end;
 }
 loc_f486:
 {
 __cost++;
   if (nondet_bool()) {
    if (L1 >= 0 && 0 >= 1 + L1) {
     goto loc_f489;
    }
   }
   if (nondet_bool()) {
    if (L1 >= 1) {
     goto loc_f489;
    }
   }
   if (nondet_bool()) {
    int M1_ = 0;
    int L1_ = -1;
    if (L1 == 0) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f486;
    }
   }
   if (nondet_bool()) {
    int S1_ = -1 + S1;
    int K1_ = -1 + K1;
    if (0 >= 1 + L1) {
     K1 = K1_;
     S1 = S1_;
     goto loc_f426;
    }
   }
  goto end;
 }
 loc_f489:
 {
 __cost++;
   if (nondet_bool()) {
    int M1_ = nondet();
    int L1_ = -1 + L1;
    if (1 >= 0) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f486;
    }
   }
   if (nondet_bool()) {
    int M1_ = 0;
    int L1_ = -1 + L1;
    if (1 >= 0) {
     L1 = L1_;
     M1 = M1_;
     goto loc_f486;
    }
   }
  goto end;
 }
 loc_f49:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = -1 + E;
    if (E >= 0) {
     E = E_;
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    int F_ = nondet();
    if (0 >= 1 + E) {
     F = F_;
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f502:
 {
 __cost++;
   if (nondet_bool()) {
    int Y1_ = -1 + Y1;
    if (Y1 >= 0) {
     Y1 = Y1_;
     goto loc_f502;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + Y1) {
     goto loc_f135;
    }
   }
  goto end;
 }
 loc_f58:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = -1 + F;
    if (F >= 0) {
     F = F_;
     goto loc_f58;
    }
   }
   if (nondet_bool()) {
    if (1131 >= A && 0 >= 1 + F) {
     goto loc_f67;
    }
   }
   if (nondet_bool()) {
    if (A >= 1133 && 0 >= 1 + F) {
     goto loc_f67;
    }
   }
   if (nondet_bool()) {
    int C2_ = nondet();
    int G_ = nondet();
    int A_ = 1132;
    if (A == 1132 && H >= 1 && 0 >= 1 + F) {
     A = A_;
     G = G_;
     C2 = C2_;
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    int C2_ = nondet();
    int A2_ = nondet();
    int A_ = 1132;
    if (A == 1132 && 0 >= H && 0 >= 1 + F) {
     A = A_;
     A2 = A2_;
     C2 = C2_;
     goto loc_f86;
    }
   }
  goto end;
 }
 loc_f67:
 {
 __cost++;
   if (nondet_bool()) {
    if (1871 >= A) {
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    if (A >= 1873) {
     goto loc_f69;
    }
   }
   if (nondet_bool()) {
    int B2_ = nondet();
    int J_ = nondet();
    int A_ = 1872;
    if (A == 1872 && H >= 1) {
     A = A_;
     J = J_;
     B2 = B2_;
     goto loc_f102;
    }
   }
   if (nondet_bool()) {
    int B2_ = nondet();
    int Z1_ = nondet();
    int K_ = nondet();
    int A_ = 1872;
    if (A == 1872 && 0 >= H) {
     A = A_;
     K = K_;
     Z1 = Z1_;
     B2 = B2_;
     goto loc_f114;
    }
   }
  goto end;
 }
 loc_f69:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int A_ = 12;
    if (A == 12) {
     A = A_;
     L = L_;
     goto loc_f124;
    }
   }
   if (nondet_bool()) {
    if (11 >= A) {
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    if (A >= 13) {
     goto loc_f135;
    }
   }
  goto end;
 }
 loc_f77:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (G >= 0) {
     G = G_;
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    int A2_ = nondet();
    if (0 >= 1 + G) {
     A2 = A2_;
     goto loc_f86;
    }
   }
  goto end;
 }
 loc_f86:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    if (H >= 1) {
     I = I_;
     goto loc_f89;
    }
   }
   if (nondet_bool()) {
    if (0 >= H) {
     goto loc_f138;
    }
   }
  goto end;
 }
 loc_f89:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = -1 + I;
    if (I >= 0) {
     I = I_;
     goto loc_f89;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + I) {
     goto loc_f138;
    }
   }
  goto end;
 }
 loc_f135:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
