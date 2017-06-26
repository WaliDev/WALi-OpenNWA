int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1, int K1, int L1, int M1, int N1, int O1, int P1, int Q1) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = 0;
    int D_ = 0;
    int C_ = 0;
    if (1 >= 0) {
     C = C_;
     D = D_;
     E = E_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f106:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     I = I_;
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    int J1_ = nondet();
    int I1_ = nondet();
    int H1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = -J1_;
    if (A >= 1 + F && X_ >= 0) {
     E = E_;
     X = X_;
     Y = Y_;
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     goto loc_f123;
    }
   }
   if (nondet_bool()) {
    int L1_ = nondet();
    int K1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int J1_ = -L1_;
    if (A >= 1 + F && 0 >= 1 + X_) {
     E = E_;
     X = X_;
     Y = Y_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     goto loc_f123;
    }
   }
  goto end;
 }
 loc_f123:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     goto loc_f123;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + F) {
     goto loc_f128;
    }
   }
  goto end;
 }
 loc_f128:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= K) {
     goto loc_f130;
    }
   }
   if (nondet_bool()) {
    if (K >= 1 + H) {
     goto loc_f142;
    }
   }
  goto end;
 }
 loc_f130:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     I = I_;
     goto loc_f130;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + F) {
     goto loc_f135;
    }
   }
  goto end;
 }
 loc_f135:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     goto loc_f135;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (A >= 1 + F) {
     K = K_;
     goto loc_f128;
    }
   }
  goto end;
 }
 loc_f142:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     goto loc_f142;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int P_ = N_ + O_;
    int M_ = C;
    if (A >= 1 + F) {
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f151;
    }
   }
  goto end;
 }
 loc_f151:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 1 + G;
    int C_ = M;
    if (M >= 1 + P) {
     C = C_;
     G = G_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    int C_ = P;
    if (P >= M) {
     C = C_;
     G = G_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f156:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= 1 && 0 >= 1 + E && F >= 1 + G) {
     goto loc_f160;
    }
   }
   if (nondet_bool()) {
    if (G >= 1 && E >= 1 && F >= 1 + G) {
     goto loc_f160;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (E == 0 && F >= 1 + G && G >= 1) {
     E = E_;
     goto loc_f179;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int G_ = -1 + G;
    int B_ = G;
    if (G >= F && G >= 1) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_f156;
    }
   }
   if (nondet_bool()) {
    if (0 >= G) {
     goto loc_f191;
    }
   }
  goto end;
 }
 loc_f160:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (F >= K) {
     K = K_;
     goto loc_f160;
    }
   }
   if (nondet_bool()) {
    if (K >= 1 + F) {
     goto loc_f165;
    }
   }
  goto end;
 }
 loc_f165:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= K) {
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    if (K >= 1 + F) {
     goto loc_f179;
    }
   }
  goto end;
 }
 loc_f167:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     I = I_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + F) {
     goto loc_f172;
    }
   }
  goto end;
 }
 loc_f172:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     goto loc_f172;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (A >= 1 + F) {
     K = K_;
     goto loc_f165;
    }
   }
  goto end;
 }
 loc_f179:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = 0;
    int K_ = 1 + K;
    if (F >= K) {
     K = K_;
     Q = Q_;
     goto loc_f179;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int G_ = -1 + G;
    int B_ = G;
    if (K >= 1 + F) {
     B = B_;
     E = E_;
     G = G_;
     goto loc_f156;
    }
   }
  goto end;
 }
 loc_f191:
 {
 __cost++;
   if (nondet_bool()) {
    int E_ = nondet();
    int B_ = 1 + G;
    if (G >= 1) {
     B = B_;
     E = E_;
     goto loc_f195;
    }
   }
   if (nondet_bool()) {
    if (0 >= G) {
     goto loc_f230;
    }
   }
  goto end;
 }
 loc_f195:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (F >= K) {
     K = K_;
     goto loc_f195;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    if (0 >= 1 + E && K >= 1 + F) {
     E = E_;
     goto loc_f202;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    if (E >= 1 && K >= 1 + F) {
     E = E_;
     goto loc_f202;
    }
   }
   if (nondet_bool()) {
    int E_ = 0;
    if (E == 0 && K >= 1 + F) {
     E = E_;
     goto loc_f223;
    }
   }
  goto end;
 }
 loc_f202:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= K) {
     goto loc_f204;
    }
   }
   if (nondet_bool()) {
    if (K >= 1 + F) {
     goto loc_f217;
    }
   }
  goto end;
 }
 loc_f204:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int A_ = 1 + A;
    if (H >= A) {
     A = A_;
     I = I_;
     goto loc_f204;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    if (A >= 1 + H) {
     X = X_;
     goto loc_f210;
    }
   }
  goto end;
 }
 loc_f210:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (H >= A) {
     A = A_;
     goto loc_f210;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (A >= 1 + H) {
     K = K_;
     goto loc_f202;
    }
   }
  goto end;
 }
 loc_f217:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (H >= K) {
     K = K_;
     goto loc_f217;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (K >= 1 + H) {
     G = G_;
     goto loc_f191;
    }
   }
  goto end;
 }
 loc_f223:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (H >= K) {
     K = K_;
     goto loc_f223;
    }
   }
   if (nondet_bool()) {
    int G_ = -1 + G;
    if (K >= 1 + H) {
     G = G_;
     goto loc_f191;
    }
   }
  goto end;
 }
 loc_f230:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1) {
     goto loc_f232;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f350;
    }
   }
  goto end;
 }
 loc_f232:
 {
 __cost++;
   if (nondet_bool()) {
    int S_ = 1;
    if (30 >= R) {
     S = S_;
     goto loc_f235;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (R >= 31) {
     A = A_;
     goto loc_f230;
    }
   }
  goto end;
 }
 loc_f235:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_f247;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int U_ = C + -Y_0;
    int T_ = -1 + B;
    int S_ = 0;
    if (B >= 1) {
     S = S_;
     T = T_;
     U = U_;
     goto loc_f247;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int U_ = nondet();
    int T_ = -1 + B;
    if (B >= 1 && 0 >= 1 + U_) {
     T = T_;
     U = U_;
     V = V_;
     goto loc_f243;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int U_ = nondet();
    int T_ = -1 + B;
    if (B >= 1 && U_ >= 1) {
     T = T_;
     U = U_;
     V = V_;
     goto loc_f243;
    }
   }
  goto end;
 }
 loc_f243:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f247;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (0 >= 1 + V) {
     B = B_;
     goto loc_f235;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (V >= 1) {
     B = B_;
     goto loc_f235;
    }
   }
  goto end;
 }
 loc_f247:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = 0;
    int I_ = 1;
    if (0 >= 1 + S) {
     I = I_;
     W = W_;
     goto loc_f250;
    }
   }
   if (nondet_bool()) {
    int W_ = 0;
    int I_ = 1;
    if (S >= 1) {
     I = I_;
     W = W_;
     goto loc_f250;
    }
   }
   if (nondet_bool()) {
    int B1_ = nondet();
    int S_ = 0;
    if (S == 0) {
     S = S_;
     B1 = B1_;
     goto loc_f274;
    }
   }
  goto end;
 }
 loc_f250:
 {
 __cost++;
   if (nondet_bool()) {
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int I_ = nondet();
    int E_ = nondet();
    if (A >= G && 0 >= 1 + Z_) {
     E = E_;
     I = I_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f263;
    }
   }
   if (nondet_bool()) {
    int Z_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int I_ = nondet();
    int E_ = nondet();
    if (A >= G && Z_ >= 1) {
     E = E_;
     I = I_;
     W = W_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f263;
    }
   }
   if (nondet_bool()) {
    int B1_ = nondet();
    if (G >= 1 + A) {
     B1 = B1_;
     goto loc_f274;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int B1_ = nondet();
    int X_ = nondet();
    int Z_ = C + -Y_0;
    if (A >= G) {
     X = X_;
     Z = Z_;
     B1 = B1_;
     goto loc_f274;
    }
   }
  goto end;
 }
 loc_f263:
 {
 __cost++;
   if (nondet_bool()) {
    int B1_ = nondet();
    int A1_ = nondet();
    int K_ = 1 + K;
    if (H >= K) {
     K = K_;
     A1 = A1_;
     B1 = B1_;
     goto loc_f263;
    }
   }
   if (nondet_bool()) {
    int G_ = 1 + G;
    if (K >= 1 + H) {
     G = G_;
     goto loc_f250;
    }
   }
  goto end;
 }
 loc_f274:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 1 + B) {
     goto loc_f283;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f283;
    }
   }
   if (nondet_bool()) {
    int A_ = B;
    if (B == A && 0 >= 1 + B1) {
     A = A_;
     goto loc_f277;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + B;
    if (B == A && B1 >= 0) {
     A = A_;
     goto loc_f230;
    }
   }
  goto end;
 }
 loc_f277:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (F >= K) {
     K = K_;
     goto loc_f277;
    }
   }
   if (nondet_bool()) {
    int A_ = -1 + A;
    if (K >= 1 + F) {
     A = A_;
     goto loc_f230;
    }
   }
  goto end;
 }
 loc_f283:
 {
 __cost++;
   if (nondet_bool()) {
    int C1_ = nondet();
    int A1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int T_ = -1 + A;
    if (29 >= R) {
     E = E_;
     T = T_;
     X = X_;
     Y = Y_;
     A1 = A1_;
     C1 = C1_;
     goto loc_f292;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int A1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int T_ = -1 + A;
    if (R >= 31) {
     E = E_;
     T = T_;
     X = X_;
     Y = Y_;
     A1 = A1_;
     C1 = C1_;
     goto loc_f292;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int A1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int T_ = -1 + A;
    int R_ = 30;
    if (R == 30) {
     E = E_;
     R = R_;
     T = T_;
     X = X_;
     Y = Y_;
     A1 = A1_;
     C1 = C1_;
     goto loc_f292;
    }
   }
  goto end;
 }
 loc_f292:
 {
 __cost++;
   if (nondet_bool()) {
    int E1_ = nondet();
    int D1_ = nondet();
    int X_ = nondet();
    int W_ = 1;
    int I_ = 1;
    if (X >= 0) {
     I = I_;
     W = W_;
     X = X_;
     D1 = D1_;
     E1 = E1_;
     goto loc_f300;
    }
   }
   if (nondet_bool()) {
    int F1_ = nondet();
    int X_ = nondet();
    int E1_ = -F1_;
    int W_ = 1;
    int I_ = 1;
    if (0 >= 1 + X) {
     I = I_;
     W = W_;
     X = X_;
     E1 = E1_;
     F1 = F1_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f300:
 {
 __cost++;
   if (nondet_bool()) {
    int B1_ = nondet();
    int A1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int I_ = nondet();
    int E_ = nondet();
    int G_ = 1 + K;
    if (T >= K) {
     E = E_;
     G = G_;
     I = I_;
     W = W_;
     X = X_;
     Y = Y_;
     A1 = A1_;
     B1 = B1_;
     goto loc_f315;
    }
   }
   if (nondet_bool()) {
    int R_ = 1 + R;
    if (K >= 1 + T) {
     R = R_;
     goto loc_f232;
    }
   }
  goto end;
 }
 loc_f315:
 {
 __cost++;
   if (nondet_bool()) {
    int C1_ = nondet();
    int B1_ = nondet();
    int G1_ = 1 + G1;
    if (F >= G1) {
     B1 = B1_;
     C1 = C1_;
     G1 = G1_;
     goto loc_f315;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int X_ = nondet();
    int B1_ = 0;
    if (G1 >= 1 + F) {
     X = X_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f331;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int I_ = nondet();
    if (0 >= 1 + Y_0 && G1 >= 1 + F) {
     I = I_;
     W = W_;
     X = X_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f331;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int C1_ = nondet();
    int B1_ = nondet();
    int X_ = nondet();
    int W_ = nondet();
    int I_ = nondet();
    if (Y_0 >= 1 && G1 >= 1 + F) {
     I = I_;
     W = W_;
     X = X_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f331;
    }
   }
  goto end;
 }
 loc_f331:
 {
 __cost++;
   if (nondet_bool()) {
    int B1_ = nondet();
    int A1_ = nondet();
    int G1_ = 1 + G1;
    if (H >= G1) {
     A1 = A1_;
     B1 = B1_;
     G1 = G1_;
     goto loc_f331;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (G1 >= 1 + H) {
     K = K_;
     goto loc_f300;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = 0;
    int E_ = 0;
    int D_ = 0;
    int B_ = 1 + G;
    if (H >= G && F >= G) {
     B = B_;
     D = D_;
     E = E_;
     I = I_;
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int E_ = 0;
    int D_ = 0;
    int B_ = 1 + G;
    if (F >= G && G >= 1 + H) {
     B = B_;
     D = D_;
     E = E_;
     I = I_;
     goto loc_f97;
    }
   }
   if (nondet_bool()) {
    if (G >= 1 + F) {
     goto loc_f156;
    }
   }
  goto end;
 }
 loc_f49:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    int D_ = D + J_;
    int A_ = 1 + A;
    if (H >= A) {
     A = A_;
     D = D_;
     J = J_;
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + H && 0 >= 1 + D) {
     goto loc_f56;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + H && D >= 1) {
     goto loc_f56;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int E_ = 0;
    int D_ = 0;
    if (D == 0 && A >= 1 + H) {
     D = D_;
     E = E_;
     I = I_;
     goto loc_f97;
    }
   }
  goto end;
 }
 loc_f56:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int A_ = 1 + A;
    if (H >= A) {
     A = A_;
     I = I_;
     goto loc_f56;
    }
   }
   if (nondet_bool()) {
    int O1_ = nondet();
    int N1_ = nondet();
    int M1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = -O1_;
    if (A >= 1 + H && X_ >= 0) {
     E = E_;
     X = X_;
     Y = Y_;
     M1 = M1_;
     N1 = N1_;
     O1 = O1_;
     goto loc_f73;
    }
   }
   if (nondet_bool()) {
    int Q1_ = nondet();
    int P1_ = nondet();
    int Y_ = nondet();
    int X_ = nondet();
    int E_ = nondet();
    int O1_ = -Q1_;
    if (A >= 1 + H && 0 >= 1 + X_) {
     E = E_;
     X = X_;
     Y = Y_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     goto loc_f73;
    }
   }
  goto end;
 }
 loc_f73:
 {
 __cost++;
   if (nondet_bool()) {
    if (F >= K) {
     goto loc_f75;
    }
   }
   if (nondet_bool()) {
    if (K >= 1 + F) {
     goto loc_f88;
    }
   }
  goto end;
 }
 loc_f75:
 {
 __cost++;
   if (nondet_bool()) {
    int I_ = nondet();
    int A_ = 1 + A;
    if (H >= A) {
     A = A_;
     I = I_;
     goto loc_f75;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    if (A >= 1 + H) {
     X = X_;
     goto loc_f81;
    }
   }
  goto end;
 }
 loc_f81:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (H >= A) {
     A = A_;
     goto loc_f81;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (A >= 1 + H) {
     K = K_;
     goto loc_f73;
    }
   }
  goto end;
 }
 loc_f88:
 {
 __cost++;
   if (nondet_bool()) {
    int A_ = 1 + A;
    if (H >= A) {
     A = A_;
     goto loc_f88;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    int E_ = 0;
    int D_ = 0;
    if (A >= 1 + H) {
     D = D_;
     E = E_;
     I = I_;
     goto loc_f97;
    }
   }
  goto end;
 }
 loc_f97:
 {
 __cost++;
   if (nondet_bool()) {
    if (H >= G && F >= 1 + G) {
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    if (H >= G && G >= 1 + F) {
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int P_ = N_ + O_;
    int M_ = C;
    if (G >= 1 + H) {
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f151;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int P_ = N_ + O_;
    int M_ = C;
    int F_ = G;
    if (G == F && H >= F) {
     F = F_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f151;
    }
   }
  goto end;
 }
 loc_f99:
 {
 __cost++;
   if (nondet_bool()) {
    int L_ = nondet();
    int D_ = D + L_;
    int A_ = 1 + A;
    if (F >= A) {
     A = A_;
     D = D_;
     L = L_;
     goto loc_f99;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + F && 0 >= 1 + D) {
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    if (A >= 1 + F && D >= 1) {
     goto loc_f106;
    }
   }
   if (nondet_bool()) {
    int O_ = nondet();
    int N_ = nondet();
    int P_ = N_ + O_;
    int M_ = C;
    int D_ = 0;
    if (D == 0 && A >= 1 + F) {
     D = D_;
     M = M_;
     N = N_;
     O = O_;
     P = P_;
     goto loc_f151;
    }
   }
  goto end;
 }
 loc_f350:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
