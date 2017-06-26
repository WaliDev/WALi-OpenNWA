int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1, int K1, int L1, int M1, int N1, int O1, int P1, int Q1, int R1, int S1, int T1, int U1, int V1, int W1, int X1, int Y1, int Z1, int A2, int B2) {
 goto loc_f0;

 loc_f0:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    if (1 >= 0) {
     F = F_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f122:
 {
 __cost++;
   if (nondet_bool()) {
    int D1_ = -1 + D1;
    if (0 >= 1 + L1) {
     D1 = D1_;
     goto loc_f96;
    }
   }
   if (nondet_bool()) {
    int D1_ = -1 + D1;
    if (L1 >= 1) {
     D1 = D1_;
     goto loc_f96;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f126;
    }
   }
  goto end;
 }
 loc_f126:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 3 + D1;
    if (H == 2 + D1 && A >= 2 + D1) {
     H = H_;
     goto loc_f126;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (A >= H && 1 + D1 >= H) {
     H = H_;
     goto loc_f126;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (A >= H && H >= 3 + D1) {
     H = H_;
     goto loc_f126;
    }
   }
   if (nondet_bool()) {
    if (H >= 1 + A) {
     goto loc_f133;
    }
   }
  goto end;
 }
 loc_f13:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= H) {
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int A_ = G;
    if (H >= 1 + G) {
     A = A_;
     Q = Q_;
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f133:
 {
 __cost++;
   if (nondet_bool()) {
    int R1_ = D1;
    if (D1 == R1 && A >= 1 + R1) {
     R1 = R1_;
     goto loc_f149;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int E1_ = 0;
    if (A >= 1 + R1 && D1 >= 1 + R1) {
     R = R_;
     S = S_;
     E1 = E1_;
     goto loc_f139;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int E1_ = 0;
    if (A >= 1 + R1 && R1 >= 1 + D1) {
     R = R_;
     S = S_;
     E1 = E1_;
     goto loc_f139;
    }
   }
   if (nondet_bool()) {
    if (R1 >= A) {
     goto loc_f46;
    }
   }
  goto end;
 }
 loc_f139:
 {
 __cost++;
   if (nondet_bool()) {
    int U1_ = nondet();
    int T1_ = nondet();
    int S1_ = nondet();
    int R1_ = -1 + A;
    int D_ = S1_ + T1_ + U1_;
    if (1 + R1 == A) {
     D = D_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f145;
    }
   }
   if (nondet_bool()) {
    int U1_ = nondet();
    int T1_ = nondet();
    int S1_ = nondet();
    int E1_ = nondet();
    int D_ = S1_ + T1_ + U1_;
    if (A >= 2 + R1) {
     D = D_;
     E1 = E1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f145;
    }
   }
   if (nondet_bool()) {
    int U1_ = nondet();
    int T1_ = nondet();
    int S1_ = nondet();
    int E1_ = nondet();
    int D_ = S1_ + T1_ + U1_;
    if (R1 >= A) {
     D = D_;
     E1 = E1_;
     S1 = S1_;
     T1 = T1_;
     U1 = U1_;
     goto loc_f145;
    }
   }
  goto end;
 }
 loc_f145:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (D == 0) {
     D = D_;
     goto loc_f149;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    if (0 >= 1 + D) {
     R = R_;
     S = S_;
     E1 = E1_;
     goto loc_f149;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    if (D >= 1) {
     R = R_;
     S = S_;
     E1 = E1_;
     goto loc_f149;
    }
   }
  goto end;
 }
 loc_f149:
 {
 __cost++;
   if (nondet_bool()) {
    int W1_ = nondet();
    int V1_ = nondet();
    int E_ = nondet();
    if (R >= 0) {
     E = E_;
     V1 = V1_;
     W1 = W1_;
     goto loc_f153;
    }
   }
   if (nondet_bool()) {
    int Y1_ = nondet();
    int X1_ = nondet();
    int E_ = -Y1_;
    if (0 >= 1 + R) {
     E = E_;
     X1 = X1_;
     Y1 = Y1_;
     goto loc_f153;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int J_ = nondet();
    int I_ = 1 + I;
    int F_ = F + J_;
    if (G >= I) {
     F = F_;
     I = I_;
     J = J_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (I >= 1 + G) {
     H = H_;
     goto loc_f13;
    }
   }
  goto end;
 }
 loc_f153:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + E) {
     goto loc_f157;
    }
   }
   if (nondet_bool()) {
    if (E >= 1) {
     goto loc_f157;
    }
   }
   if (nondet_bool()) {
    int R1_ = 1 + R1;
    int E_ = 0;
    if (E == 0) {
     E = E_;
     R1 = R1_;
     goto loc_f133;
    }
   }
  goto end;
 }
 loc_f157:
 {
 __cost++;
   if (nondet_bool()) {
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int R1_ = B;
    int D1_ = B;
    int R_ = R + E;
    if (D1 == R1 && B == R1) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
     E1 = E1_;
     R1 = R1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int R1_ = D1;
    int R_ = R + E;
    if (D1 == R1 && R1 >= 1 + B) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     E1 = E1_;
     R1 = R1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int R1_ = D1;
    int R_ = R + E;
    if (D1 == R1 && B >= 1 + R1) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     E1 = E1_;
     R1 = R1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int R_ = R + E;
    if (D1 >= 1 + R1) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     E1 = E1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int R_ = R + E;
    if (R1 >= 1 + D1) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     E1 = E1_;
     goto loc_f167;
    }
   }
  goto end;
 }
 loc_f167:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = nondet();
    int R1_ = -1 + A;
    int I_ = 1 + I;
    if (1 + R1 == A && A >= I) {
     I = I_;
     R = R_;
     R1 = R1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int I_ = 1 + I;
    if (A >= I && A >= 2 + R1) {
     I = I_;
     R = R_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int I_ = 1 + I;
    if (A >= I && R1 >= A) {
     I = I_;
     R = R_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Z1_ = A;
    if (2 + R1 >= A && I >= 1 + A) {
     Z1 = Z1_;
     goto loc_f180;
    }
   }
   if (nondet_bool()) {
    int Z1_ = 3 + R1;
    if (A >= 3 + R1 && I >= 1 + A) {
     Z1 = Z1_;
     goto loc_f180;
    }
   }
  goto end;
 }
 loc_f180:
 {
 __cost++;
   if (nondet_bool()) {
    int R_ = nondet();
    int R1_ = -1 + A;
    int H_ = 1 + H;
    if (1 + R1 == A && Z1 >= H) {
     H = H_;
     R = R_;
     R1 = R1_;
     goto loc_f180;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int H_ = 1 + H;
    if (Z1 >= H && A >= 2 + R1) {
     H = H_;
     R = R_;
     goto loc_f180;
    }
   }
   if (nondet_bool()) {
    int R_ = nondet();
    int H_ = 1 + H;
    if (Z1 >= H && R1 >= A) {
     H = H_;
     R = R_;
     goto loc_f180;
    }
   }
   if (nondet_bool()) {
    int R1_ = 1 + R1;
    if (H >= 1 + Z1) {
     R1 = R1_;
     goto loc_f133;
    }
   }
  goto end;
 }
 loc_f25:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (A >= 1) {
     C = C_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f198;
    }
   }
  goto end;
 }
 loc_f28:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= B) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int E_ = K_ + L_;
    if (B >= 2 && 0 >= 1 + K_ + L_) {
     E = E_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = nondet();
    int E_ = K_ + L_;
    if (B >= 2 && K_ + L_ >= 1) {
     E = E_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f36;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    int L_ = nondet();
    int K_ = -L_;
    int E_ = F;
    if (B >= 2) {
     E = E_;
     K = K_;
     L = L_;
     M = M_;
     goto loc_f36;
    }
   }
  goto end;
 }
 loc_f36:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f40;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (0 >= 1 + M) {
     B = B_;
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (M >= 1) {
     B = B_;
     goto loc_f28;
    }
   }
  goto end;
 }
 loc_f40:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int N_ = A;
    int B_ = A;
    int A_ = -1 + A;
    if (A == B) {
     A = A_;
     B = B_;
     D = D_;
     N = N_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    if (A >= 1 + B) {
     D = D_;
     O = O_;
     P = P_;
     goto loc_f49;
    }
   }
   if (nondet_bool()) {
    int P_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    if (B >= 1 + A) {
     D = D_;
     O = O_;
     P = P_;
     goto loc_f49;
    }
   }
  goto end;
 }
 loc_f46:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2 + B && 30 >= C) {
     goto loc_f28;
    }
   }
   if (nondet_bool()) {
    if (A >= 2 + B && C >= 31) {
     goto loc_f25;
    }
   }
   if (nondet_bool()) {
    if (1 + B >= A) {
     goto loc_f25;
    }
   }
  goto end;
 }
 loc_f49:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2 + B) {
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_f78;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    int W_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Y_ = D + Q + R_ + X_;
    int V_ = R_ + X_;
    int D_ = D + Q;
    int B_ = -1 + A;
    if (1 + B == A && S_ >= 0 && R_ >= 0) {
     B = B_;
     D = D_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    int Z_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int Y_ = D + Q + R_ + -Z_;
    int X_ = -Z_;
    int V_ = R_ + -Z_;
    int D_ = D + Q;
    int B_ = -1 + A;
    if (1 + B == A && S_ >= 0 && 0 >= 1 + R_) {
     B = B_;
     D = D_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f66;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int B1_ = D + Q + R_;
    int D_ = D + Q;
    int B_ = -1 + A;
    int A_ = -2 + A;
    if (1 + B == A && 0 >= 1 + S_) {
     A = A_;
     B = B_;
     D = D_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f46;
    }
   }
  goto end;
 }
 loc_f66:
 {
 __cost++;
   if (nondet_bool()) {
    int A1_ = 0;
    int V_ = 0;
    int A_ = -2 + A;
    if (V == 0) {
     A = A_;
     V = V_;
     A1 = A1_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int A1_ = 0;
    int A_ = -2 + A;
    if (0 >= 1 + V) {
     A = A_;
     A1 = A1_;
     goto loc_f46;
    }
   }
   if (nondet_bool()) {
    int A1_ = 0;
    int A_ = -2 + A;
    if (V >= 1) {
     A = A_;
     A1 = A1_;
     goto loc_f46;
    }
   }
  goto end;
 }
 loc_f78:
 {
 __cost++;
   if (nondet_bool()) {
    if (29 >= C) {
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    if (C >= 31) {
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int C_ = 30;
    if (C == 30) {
     C = C_;
     goto loc_f80;
    }
   }
  goto end;
 }
 loc_f80:
 {
 __cost++;
   if (nondet_bool()) {
    if (9 >= C) {
     goto loc_f82;
    }
   }
   if (nondet_bool()) {
    if (C >= 11) {
     goto loc_f82;
    }
   }
   if (nondet_bool()) {
    int Q_ = Q + D;
    int C_ = 10;
    if (C == 10) {
     C = C_;
     Q = Q_;
     goto loc_f84;
    }
   }
  goto end;
 }
 loc_f82:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = Q + D;
    int C_ = 20;
    if (C == 20) {
     C = C_;
     Q = Q_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (19 >= C) {
     C = C_;
     goto loc_f96;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (C >= 21) {
     C = C_;
     goto loc_f96;
    }
   }
  goto end;
 }
 loc_f84:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (A >= H) {
     H = H_;
     goto loc_f84;
    }
   }
   if (nondet_bool()) {
    int B2_ = nondet();
    int A2_ = nondet();
    int P_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int E_ = A2_ + B2_;
    int C_ = 1 + C;
    if (H >= 1 + A) {
     C = C_;
     D = D_;
     E = E_;
     O = O_;
     P = P_;
     A2 = A2_;
     B2 = B2_;
     goto loc_f96;
    }
   }
  goto end;
 }
 loc_f96:
 {
 __cost++;
   if (nondet_bool()) {
    int Q1_ = nondet();
    int P1_ = nondet();
    int O1_ = nondet();
    int N1_ = nondet();
    int M1_ = nondet();
    int L1_ = nondet();
    int K1_ = nondet();
    int J1_ = nondet();
    int I1_ = nondet();
    int H1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int E_ = F1_ + G1_ + H1_;
    if (B >= 1 + D1 && D1 >= B) {
     E = E_;
     R = R_;
     S = S_;
     V = V_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     N1 = N1_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     goto loc_f122;
    }
   }
   if (nondet_bool()) {
    int Q1_ = nondet();
    int P1_ = nondet();
    int O1_ = nondet();
    int N1_ = nondet();
    int M1_ = nondet();
    int L1_ = nondet();
    int K1_ = nondet();
    int J1_ = nondet();
    int I1_ = nondet();
    int H1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int E_ = F1_ + G1_ + H1_;
    if (D1 >= 1 + B) {
     E = E_;
     R = R_;
     S = S_;
     V = V_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     H1 = H1_;
     I1 = I1_;
     J1 = J1_;
     K1 = K1_;
     L1 = L1_;
     M1 = M1_;
     N1 = N1_;
     O1 = O1_;
     P1 = P1_;
     Q1 = Q1_;
     goto loc_f122;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + D1) {
     goto loc_f126;
    }
   }
   if (nondet_bool()) {
    int H1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int D1_ = B;
    int E_ = F1_ + G1_ + H1_;
    if (B == D1) {
     E = E_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     H1 = H1_;
     goto loc_f126;
    }
   }
  goto end;
 }
 loc_f198:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
