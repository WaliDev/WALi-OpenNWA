int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1, int K1, int L1, int M1, int N1, int O1, int P1, int Q1, int R1, int S1, int T1, int U1, int V1, int W1, int X1, int Y1, int Z1, int A2, int B2, int C2, int D2, int E2, int F2, int G2, int H2, int I2, int J2, int K2, int L2, int M2, int N2, int O2, int P2, int Q2, int R2) {
 goto loc_f3;

 loc_f102:
 {
 __cost++;
   if (nondet_bool()) {
    int V_ = nondet();
    int R_ = 9;
    if (U >= 1) {
     R = R_;
     V = V_;
     goto loc_f74;
    }
   }
   if (nondet_bool()) {
    int G1_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     G1 = G1_;
     goto loc_f123;
    }
   }
  goto end;
 }
 loc_f107:
 {
 __cost++;
   if (nondet_bool()) {
    int U1_ = 1;
    int T1_ = X;
    int S1_ = S;
    int R1_ = W;
    int Q1_ = R;
    int P1_ = T;
    int O1_ = V;
    int U_ = 1;
    if (W >= 1) {
     U = U_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f117;
    }
   }
   if (nondet_bool()) {
    int U1_ = 0;
    int T1_ = X;
    int S1_ = S;
    int R1_ = W;
    int Q1_ = R;
    int P1_ = T;
    int O1_ = V;
    int U_ = 0;
    if (0 >= W) {
     U = U_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f117;
    }
   }
  goto end;
 }
 loc_f117:
 {
 __cost++;
   if (nondet_bool()) {
    int F1_ = G1;
    if (U >= 1) {
     F1 = F1_;
     goto loc_f123;
    }
   }
   if (nondet_bool()) {
    int G1_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     G1 = G1_;
     goto loc_f123;
    }
   }
  goto end;
 }
 loc_f123:
 {
 __cost++;
   if (nondet_bool()) {
    int A2_ = H;
    int Z1_ = G;
    int Y1_ = F;
    int X1_ = A;
    int W1_ = E;
    int V1_ = D;
    if (0 >= F1) {
     V1 = V1_;
     W1 = W1_;
     X1 = X1_;
     Y1 = Y1_;
     Z1 = Z1_;
     A2 = A2_;
     goto loc_f151;
    }
   }
  goto end;
 }
 loc_f132:
 {
 __cost++;
   if (nondet_bool()) {
    int X1_ = 1;
    if (1 >= 0) {
     X1 = X1_;
     goto loc_f135;
    }
   }
  goto end;
 }
 loc_f135:
 {
 __cost++;
   if (nondet_bool()) {
    int W1_ = nondet();
    int Z1_ = 1 + Z1;
    int X1_ = 1;
    if (W1_ >= 1 && 0 >= Z1) {
     W1 = W1_;
     X1 = X1_;
     Z1 = Z1_;
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    int W1_ = nondet();
    int Z1_ = -1 + Z1;
    int X1_ = 1;
    if (0 >= W1_ && 0 >= Z1) {
     W1 = W1_;
     X1 = X1_;
     Z1 = Z1_;
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    int V1_ = nondet();
    int X1_ = 9;
    if (0 >= Z1) {
     V1 = V1_;
     X1 = X1_;
     goto loc_f151;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f151:
 {
 __cost++;
   if (nondet_bool()) {
    int Y1_ = 0;
    int X1_ = 10;
    int E1_ = 0;
    int D1_ = A2;
    int C1_ = Z1;
    int B1_ = 0;
    int A1_ = 10;
    int Z_ = W1;
    int Y_ = V1;
    int U_ = 0;
    if (V1 >= 1) {
     U = U_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     X1 = X1_;
     Y1 = Y1_;
     goto loc_f164;
    }
   }
   if (nondet_bool()) {
    int M1_ = A2;
    int L1_ = Z1;
    int K1_ = Y1;
    int J1_ = X1;
    int I1_ = W1;
    int H1_ = V1;
    if (0 >= V1) {
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     goto loc_f172;
    }
   }
  goto end;
 }
 loc_f164:
 {
 __cost++;
   if (nondet_bool()) {
    int B2_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     B2 = B2_;
     goto loc_f200;
    }
   }
   if (nondet_bool()) {
    int M1_ = A2;
    int L1_ = Z1;
    int K1_ = Y1;
    int J1_ = X1;
    int I1_ = W1;
    int H1_ = V1;
    if (U >= 1) {
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     goto loc_f172;
    }
   }
  goto end;
 }
 loc_f172:
 {
 __cost++;
   if (nondet_bool()) {
    int N1_ = 1;
    int U_ = 1;
    if (K1 >= 1) {
     U = U_;
     N1 = N1_;
     goto loc_f179;
    }
   }
   if (nondet_bool()) {
    int N1_ = 0;
    int U_ = 0;
    if (0 >= K1) {
     U = U_;
     N1 = N1_;
     goto loc_f179;
    }
   }
  goto end;
 }
 loc_f179:
 {
 __cost++;
   if (nondet_bool()) {
    int V1_ = nondet();
    int X1_ = 9;
    if (U >= 1) {
     V1 = V1_;
     X1 = X1_;
     goto loc_f151;
    }
   }
   if (nondet_bool()) {
    int B2_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     B2 = B2_;
     goto loc_f200;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int G_ = 1 + G;
    int A_ = 5;
    if (E_ >= 1 && 0 >= G) {
     A = A_;
     E = E_;
     G = G_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int G_ = -1 + G;
    int A_ = 6;
    if (0 >= E_ && 0 >= G) {
     A = A_;
     E = E_;
     G = G_;
     goto loc_f29;
    }
   }
   if (nondet_bool()) {
    int D_ = nondet();
    int A_ = 9;
    if (0 >= G) {
     A = A_;
     D = D_;
     goto loc_f48;
    }
   }
  goto end;
 }
 loc_f184:
 {
 __cost++;
   if (nondet_bool()) {
    int U1_ = 1;
    int T1_ = A2;
    int S1_ = Z1;
    int R1_ = Y1;
    int Q1_ = X1;
    int P1_ = W1;
    int O1_ = V1;
    int U_ = 1;
    if (Y1 >= 1) {
     U = U_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f194;
    }
   }
   if (nondet_bool()) {
    int U1_ = 0;
    int T1_ = A2;
    int S1_ = Z1;
    int R1_ = Y1;
    int Q1_ = X1;
    int P1_ = W1;
    int O1_ = V1;
    int U_ = 0;
    if (0 >= Y1) {
     U = U_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f194;
    }
   }
  goto end;
 }
 loc_f194:
 {
 __cost++;
   if (nondet_bool()) {
    int F1_ = B2;
    if (U >= 1) {
     F1 = F1_;
     goto loc_f200;
    }
   }
   if (nondet_bool()) {
    int B2_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     B2 = B2_;
     goto loc_f200;
    }
   }
  goto end;
 }
 loc_f200:
 {
 __cost++;
   if (nondet_bool()) {
    int C2_ = nondet();
    int D_ = nondet();
    int A_ = 9;
    if (0 >= C2_ && 0 >= F1 && 0 >= I) {
     A = A_;
     D = D_;
     C2 = C2_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int C2_ = nondet();
    int D_ = nondet();
    int I2_ = H;
    int H2_ = G;
    int G2_ = F;
    int F2_ = A;
    int E2_ = E;
    int D2_ = D;
    int I_ = 1;
    int A_ = 9;
    if (C2_ >= 1 && 0 >= F1 && 0 >= I) {
     A = A_;
     D = D_;
     I = I_;
     C2 = C2_;
     D2 = D2_;
     E2 = E2_;
     F2 = F2_;
     G2 = G2_;
     H2 = H2_;
     I2 = I2_;
     goto loc_f48;
    }
   }
   if (nondet_bool()) {
    int R2_ = 0;
    int Q2_ = 0;
    if (I >= 1 && 0 >= F1) {
     Q2 = Q2_;
     R2 = R2_;
     goto loc_f666666;
    }
   }
  goto end;
 }
 loc_f219:
 {
 __cost++;
   if (nondet_bool()) {
    int M2_ = nondet();
    int O2_ = H;
    int N2_ = F;
    int L2_ = E;
    int K2_ = G;
    int J2_ = 9;
    if (1 >= 0) {
     J2 = J2_;
     K2 = K2_;
     L2 = L2_;
     M2 = M2_;
     N2 = N2_;
     O2 = O2_;
     goto loc_f242;
    }
   }
  goto end;
 }
 loc_f223:
 {
 __cost++;
   if (nondet_bool()) {
    int J2_ = 1;
    if (1 >= 0) {
     J2 = J2_;
     goto loc_f226;
    }
   }
  goto end;
 }
 loc_f226:
 {
 __cost++;
   if (nondet_bool()) {
    int L2_ = nondet();
    int K2_ = 1 + K2;
    int J2_ = 1;
    if (L2_ >= 1 && 0 >= K2) {
     J2 = J2_;
     K2 = K2_;
     L2 = L2_;
     goto loc_f226;
    }
   }
   if (nondet_bool()) {
    int L2_ = nondet();
    int K2_ = -1 + K2;
    int J2_ = 1;
    if (0 >= L2_ && 0 >= K2) {
     J2 = J2_;
     K2 = K2_;
     L2 = L2_;
     goto loc_f226;
    }
   }
   if (nondet_bool()) {
    int M2_ = nondet();
    int J2_ = 9;
    if (0 >= K2) {
     J2 = J2_;
     M2 = M2_;
     goto loc_f242;
    }
   }
  goto end;
 }
 loc_f242:
 {
 __cost++;
   if (nondet_bool()) {
    int N2_ = 0;
    int J2_ = 10;
    int E1_ = 0;
    int D1_ = O2;
    int C1_ = K2;
    int B1_ = 0;
    int A1_ = 10;
    int Z_ = L2;
    int Y_ = M2;
    int U_ = 0;
    if (M2 >= 1) {
     U = U_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     J2 = J2_;
     N2 = N2_;
     goto loc_f255;
    }
   }
   if (nondet_bool()) {
    int M1_ = O2;
    int L1_ = K2;
    int K1_ = N2;
    int J1_ = J2;
    int I1_ = L2;
    int H1_ = M2;
    if (0 >= M2) {
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     goto loc_f263;
    }
   }
  goto end;
 }
 loc_f255:
 {
 __cost++;
   if (nondet_bool()) {
    int P2_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     P2 = P2_;
     goto loc_f291;
    }
   }
   if (nondet_bool()) {
    int M1_ = O2;
    int L1_ = K2;
    int K1_ = N2;
    int J1_ = J2;
    int I1_ = L2;
    int H1_ = M2;
    if (U >= 1) {
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     goto loc_f263;
    }
   }
  goto end;
 }
 loc_f263:
 {
 __cost++;
   if (nondet_bool()) {
    int N1_ = 1;
    int U_ = 1;
    if (K1 >= 1) {
     U = U_;
     N1 = N1_;
     goto loc_f270;
    }
   }
   if (nondet_bool()) {
    int N1_ = 0;
    int U_ = 0;
    if (0 >= K1) {
     U = U_;
     N1 = N1_;
     goto loc_f270;
    }
   }
  goto end;
 }
 loc_f270:
 {
 __cost++;
   if (nondet_bool()) {
    int M2_ = nondet();
    int J2_ = 9;
    if (U >= 1) {
     J2 = J2_;
     M2 = M2_;
     goto loc_f242;
    }
   }
   if (nondet_bool()) {
    int P2_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     P2 = P2_;
     goto loc_f291;
    }
   }
  goto end;
 }
 loc_f275:
 {
 __cost++;
   if (nondet_bool()) {
    int U1_ = 1;
    int T1_ = O2;
    int S1_ = K2;
    int R1_ = N2;
    int Q1_ = J2;
    int P1_ = L2;
    int O1_ = M2;
    int U_ = 1;
    if (N2 >= 1) {
     U = U_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f285;
    }
   }
   if (nondet_bool()) {
    int U1_ = 0;
    int T1_ = O2;
    int S1_ = K2;
    int R1_ = N2;
    int Q1_ = J2;
    int P1_ = L2;
    int O1_ = M2;
    int U_ = 0;
    if (0 >= N2) {
     U = U_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f285;
    }
   }
  goto end;
 }
 loc_f285:
 {
 __cost++;
   if (nondet_bool()) {
    int F1_ = P2;
    if (U >= 1) {
     F1 = F1_;
     goto loc_f291;
    }
   }
   if (nondet_bool()) {
    int P2_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     P2 = P2_;
     goto loc_f291;
    }
   }
  goto end;
 }
 loc_f29:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = nondet();
    int A_ = 1;
    if (0 >= J && 0 >= K_) {
     A = A_;
     K = K_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int K_ = nondet();
    int Q_ = H;
    int P_ = G;
    int O_ = F;
    int N_ = A;
    int M_ = E;
    int L_ = D;
    int J_ = 1;
    int A_ = 1;
    if (0 >= J && K_ >= 1) {
     A = A_;
     J = J_;
     K = K_;
     L = L_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     Q = Q_;
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    int R2_ = 0;
    int Q2_ = 0;
    if (J >= 1) {
     Q2 = Q2_;
     R2 = R2_;
     goto loc_f666666;
    }
   }
  goto end;
 }
 loc_f291:
 {
 __cost++;
   if (nondet_bool()) {
    int R2_ = Q2;
    if (0 >= F1 && Q2 >= 2) {
     R2 = R2_;
     goto loc_f666666;
    }
   }
   if (nondet_bool()) {
    int R2_ = Q2;
    if (0 >= F1 && 0 >= Q2) {
     R2 = R2_;
     goto loc_f666666;
    }
   }
  goto end;
 }
 loc_f3:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int G_ = nondet();
    int E_ = nondet();
    int D_ = nondet();
    int J_ = 0;
    int I_ = 0;
    int F_ = 0;
    int C_ = 0;
    int B_ = 0;
    int A_ = 1;
    if (1 >= 0) {
     A = A_;
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     F = F_;
     G = G_;
     H = H_;
     I = I_;
     J = J_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f48:
 {
 __cost++;
   if (nondet_bool()) {
    int D1_ = H;
    int C1_ = G;
    int B1_ = 0;
    int A1_ = 10;
    int Z_ = E;
    int Y_ = D;
    int X_ = H;
    int W_ = 0;
    int V_ = D;
    int T_ = E;
    int S_ = G;
    int R_ = 10;
    int F_ = 0;
    int A_ = 10;
    if (D >= 1) {
     A = A_;
     F = F_;
     R = R_;
     S = S_;
     T = T_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int A2_ = H;
    int Z1_ = G;
    int Y1_ = F;
    int X1_ = A;
    int W1_ = E;
    int V1_ = D;
    if (0 >= D) {
     V1 = V1_;
     W1 = W1_;
     X1 = X1_;
     Y1 = Y1_;
     Z1 = Z1_;
     A2 = A2_;
     goto loc_f151;
    }
   }
  goto end;
 }
 loc_f55:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = 1;
    if (1 >= 0) {
     R = R_;
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f58:
 {
 __cost++;
   if (nondet_bool()) {
    int T_ = nondet();
    int S_ = 1 + S;
    int R_ = 1;
    if (T_ >= 1 && 0 >= S) {
     R = R_;
     S = S_;
     T = T_;
     goto loc_f58;
    }
   }
   if (nondet_bool()) {
    int T_ = nondet();
    int S_ = -1 + S;
    int R_ = 1;
    if (0 >= T_ && 0 >= S) {
     R = R_;
     S = S_;
     T = T_;
     goto loc_f58;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int R_ = 9;
    if (0 >= S) {
     R = R_;
     V = V_;
     goto loc_f74;
    }
   }
  goto end;
 }
 loc_f74:
 {
 __cost++;
   if (nondet_bool()) {
    int D1_ = X;
    int C1_ = S;
    int B1_ = 0;
    int A1_ = 10;
    int Z_ = T;
    int Y_ = V;
    int W_ = 0;
    int R_ = 10;
    if (V >= 1) {
     R = R_;
     W = W_;
     Y = Y_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     D1 = D1_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int M1_ = X;
    int L1_ = S;
    int K1_ = W;
    int J1_ = R;
    int I1_ = T;
    int H1_ = V;
    if (0 >= V) {
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     goto loc_f95;
    }
   }
  goto end;
 }
 loc_f80:
 {
 __cost++;
   if (nondet_bool()) {
    int E1_ = 1;
    int U_ = 1;
    if (B1 >= 1) {
     U = U_;
     E1 = E1_;
     goto loc_f87;
    }
   }
   if (nondet_bool()) {
    int E1_ = 0;
    int U_ = 0;
    if (0 >= B1) {
     U = U_;
     E1 = E1_;
     goto loc_f87;
    }
   }
  goto end;
 }
 loc_f87:
 {
 __cost++;
   if (nondet_bool()) {
    int G1_ = 0;
    int F1_ = 0;
    if (0 >= U) {
     F1 = F1_;
     G1 = G1_;
     goto loc_f123;
    }
   }
   if (nondet_bool()) {
    int M1_ = X;
    int L1_ = S;
    int K1_ = W;
    int J1_ = R;
    int I1_ = T;
    int H1_ = V;
    if (U >= 1) {
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     goto loc_f95;
    }
   }
  goto end;
 }
 loc_f95:
 {
 __cost++;
   if (nondet_bool()) {
    int N1_ = 1;
    int U_ = 1;
    if (K1 >= 1) {
     U = U_;
     N1 = N1_;
     goto loc_f102;
    }
   }
   if (nondet_bool()) {
    int N1_ = 0;
    int U_ = 0;
    if (0 >= K1) {
     U = U_;
     N1 = N1_;
     goto loc_f102;
    }
   }
  goto end;
 }
 loc_f666666:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
