int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1, int D1, int E1, int F1, int G1, int H1, int I1, int J1, int K1, int L1, int M1, int N1, int O1, int P1, int Q1, int R1, int S1, int T1, int U1, int V1, int W1, int X1, int Y1, int Z1, int A2) {
 goto loc_f2;

 loc_f119:
 {
 __cost++;
   if (nondet_bool()) {
    int C1_ = -1 + C1;
    if (0 >= 1 + K1) {
     C1 = C1_;
     goto loc_f93;
    }
   }
   if (nondet_bool()) {
    int C1_ = -1 + C1;
    if (K1 >= 1) {
     C1 = C1_;
     goto loc_f93;
    }
   }
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f124;
    }
   }
  goto end;
 }
 loc_f124:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 3 + C1;
    if (H == 2 + C1 && A >= 2 + C1) {
     H = H_;
     goto loc_f124;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (A >= H && 1 + C1 >= H) {
     H = H_;
     goto loc_f124;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (A >= H && H >= 3 + C1) {
     H = H_;
     goto loc_f124;
    }
   }
   if (nondet_bool()) {
    if (H >= 1 + A) {
     goto loc_f132;
    }
   }
  goto end;
 }
 loc_f132:
 {
 __cost++;
   if (nondet_bool()) {
    int Q1_ = C1;
    if (C1 == Q1 && A >= 1 + Q1) {
     Q1 = Q1_;
     goto loc_f148;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int D1_ = 0;
    if (A >= 1 + Q1 && C1 >= 1 + Q1) {
     R = R_;
     S = S_;
     D1 = D1_;
     goto loc_f138;
    }
   }
   if (nondet_bool()) {
    int S_ = nondet();
    int R_ = nondet();
    int D1_ = 0;
    if (A >= 1 + Q1 && Q1 >= 1 + C1) {
     R = R_;
     S = S_;
     D1 = D1_;
     goto loc_f138;
    }
   }
   if (nondet_bool()) {
    if (Q1 >= A) {
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f138:
 {
 __cost++;
   if (nondet_bool()) {
    int T1_ = nondet();
    int S1_ = nondet();
    int R1_ = nondet();
    int Q1_ = -1 + A;
    int D_ = R1_ + S1_ + T1_;
    if (1 + Q1 == A) {
     D = D_;
     Q1 = Q1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     goto loc_f144;
    }
   }
   if (nondet_bool()) {
    int T1_ = nondet();
    int S1_ = nondet();
    int R1_ = nondet();
    int D1_ = nondet();
    int D_ = R1_ + S1_ + T1_;
    if (A >= 2 + Q1) {
     D = D_;
     D1 = D1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     goto loc_f144;
    }
   }
   if (nondet_bool()) {
    int T1_ = nondet();
    int S1_ = nondet();
    int R1_ = nondet();
    int D1_ = nondet();
    int D_ = R1_ + S1_ + T1_;
    if (Q1 >= A) {
     D = D_;
     D1 = D1_;
     R1 = R1_;
     S1 = S1_;
     T1 = T1_;
     goto loc_f144;
    }
   }
  goto end;
 }
 loc_f144:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = 0;
    if (D == 0) {
     D = D_;
     goto loc_f148;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int D1_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    if (R_ >= Y_0 && D * Y_0 + Y_0 >= 1 + R && R >= D * Y_0 && Y_1 >= R_ && D * Y_1 + Y_1 >= 1 + R && R >= D * Y_1 && 0 >= 1 + D && S_ >= Y_2 && D * Y_2 + Y_2 >= 1 + S && S >= D * Y_2 && Y_3 >= S_ && D * Y_3 + Y_3 >= 1 + S && S >= D * Y_3 && D1_ >= Y_4 && D * Y_4 + Y_4 >= 1 + D1 && D1 >= D * Y_4 && Y_5 >= D1_ && D * Y_5 + Y_5 >= 1 + D1 && D1 >= D * Y_5) {
     R = R_;
     S = S_;
     D1 = D1_;
     goto loc_f148;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int D1_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    if (R_ >= Y_0 && D * Y_0 + Y_0 >= 1 + R && R >= D * Y_0 && Y_1 >= R_ && D * Y_1 + Y_1 >= 1 + R && R >= D * Y_1 && D >= 1 && S_ >= Y_2 && D * Y_2 + Y_2 >= 1 + S && S >= D * Y_2 && Y_3 >= S_ && D * Y_3 + Y_3 >= 1 + S && S >= D * Y_3 && D1_ >= Y_4 && D * Y_4 + Y_4 >= 1 + D1 && D1 >= D * Y_4 && Y_5 >= D1_ && D * Y_5 + Y_5 >= 1 + D1 && D1 >= D * Y_5) {
     R = R_;
     S = S_;
     D1 = D1_;
     goto loc_f148;
    }
   }
  goto end;
 }
 loc_f148:
 {
 __cost++;
   if (nondet_bool()) {
    int V1_ = nondet();
    int U1_ = nondet();
    int E_ = nondet();
    if (R >= 0) {
     E = E_;
     U1 = U1_;
     V1 = V1_;
     goto loc_f152;
    }
   }
   if (nondet_bool()) {
    int X1_ = nondet();
    int W1_ = nondet();
    int E_ = -X1_;
    if (0 >= 1 + R) {
     E = E_;
     W1 = W1_;
     X1 = X1_;
     goto loc_f152;
    }
   }
  goto end;
 }
 loc_f152:
 {
 __cost++;
   if (nondet_bool()) {
    if (0 >= 1 + E) {
     goto loc_f156;
    }
   }
   if (nondet_bool()) {
    if (E >= 1) {
     goto loc_f156;
    }
   }
   if (nondet_bool()) {
    int Q1_ = 1 + Q1;
    int E_ = 0;
    if (E == 0) {
     E = E_;
     Q1 = Q1_;
     goto loc_f132;
    }
   }
  goto end;
 }
 loc_f156:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int Q1_ = B;
    int C1_ = B;
    int R_ = R + E;
    if (C1 == Q1 && B == Q1 && O_ >= Y_0 && E * Y_0 + Y_0 >= 1 + S && S >= E * Y_0 && Y_1 >= O_ && E * Y_1 + Y_1 >= 1 + S && S >= E * Y_1 && D_ >= Y_2 && E * Y_2 + Y_2 >= 1 + R + E && R + E >= E * Y_2 && Y_3 >= D_ && E * Y_3 + Y_3 >= 1 + R + E && R + E >= E * Y_3 && S_ >= Y_4 && R * Y_4 + E * Y_4 + Y_4 >= 1 + S && S >= R * Y_4 + E * Y_4 && Y_5 >= S_ && R * Y_5 + E * Y_5 + Y_5 >= 1 + S && S >= R * Y_5 + E * Y_5 && V_ >= Y_6 && E * Y_6 + Y_6 >= 1 + D1 && D1 >= E * Y_6 && Y_7 >= V_ && E * Y_7 + Y_7 >= 1 + D1 && D1 >= E * Y_7 && D1_ >= Y_8 && R * Y_8 + E * Y_8 + Y_8 >= 1 + D1 && D1 >= R * Y_8 + E * Y_8 && Y_9 >= D1_ && R * Y_9 + E * Y_9 + Y_9 >= 1 + D1 && D1 >= R * Y_9 + E * Y_9) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     C1 = C1_;
     D1 = D1_;
     Q1 = Q1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int Q1_ = C1;
    int R_ = R + E;
    if (C1 == Q1 && O_ >= Y_0 && E * Y_0 + Y_0 >= 1 + S && S >= E * Y_0 && Y_1 >= O_ && E * Y_1 + Y_1 >= 1 + S && S >= E * Y_1 && D_ >= Y_2 && E * Y_2 + Y_2 >= 1 + R + E && R + E >= E * Y_2 && Y_3 >= D_ && E * Y_3 + Y_3 >= 1 + R + E && R + E >= E * Y_3 && S_ >= Y_4 && R * Y_4 + E * Y_4 + Y_4 >= 1 + S && S >= R * Y_4 + E * Y_4 && Y_5 >= S_ && R * Y_5 + E * Y_5 + Y_5 >= 1 + S && S >= R * Y_5 + E * Y_5 && V_ >= Y_6 && E * Y_6 + Y_6 >= 1 + D1 && D1 >= E * Y_6 && Y_7 >= V_ && E * Y_7 + Y_7 >= 1 + D1 && D1 >= E * Y_7 && D1_ >= Y_8 && R * Y_8 + E * Y_8 + Y_8 >= 1 + D1 && D1 >= R * Y_8 + E * Y_8 && Y_9 >= D1_ && R * Y_9 + E * Y_9 + Y_9 >= 1 + D1 && D1 >= R * Y_9 + E * Y_9 && Q1 >= 1 + B) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
     Q1 = Q1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int Q1_ = C1;
    int R_ = R + E;
    if (C1 == Q1 && O_ >= Y_0 && E * Y_0 + Y_0 >= 1 + S && S >= E * Y_0 && Y_1 >= O_ && E * Y_1 + Y_1 >= 1 + S && S >= E * Y_1 && D_ >= Y_2 && E * Y_2 + Y_2 >= 1 + R + E && R + E >= E * Y_2 && Y_3 >= D_ && E * Y_3 + Y_3 >= 1 + R + E && R + E >= E * Y_3 && S_ >= Y_4 && R * Y_4 + E * Y_4 + Y_4 >= 1 + S && S >= R * Y_4 + E * Y_4 && Y_5 >= S_ && R * Y_5 + E * Y_5 + Y_5 >= 1 + S && S >= R * Y_5 + E * Y_5 && V_ >= Y_6 && E * Y_6 + Y_6 >= 1 + D1 && D1 >= E * Y_6 && Y_7 >= V_ && E * Y_7 + Y_7 >= 1 + D1 && D1 >= E * Y_7 && D1_ >= Y_8 && R * Y_8 + E * Y_8 + Y_8 >= 1 + D1 && D1 >= R * Y_8 + E * Y_8 && Y_9 >= D1_ && R * Y_9 + E * Y_9 + Y_9 >= 1 + D1 && D1 >= R * Y_9 + E * Y_9 && B >= 1 + Q1) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
     Q1 = Q1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int R_ = R + E;
    if (O_ >= Y_0 && E * Y_0 + Y_0 >= 1 + S && S >= E * Y_0 && Y_1 >= O_ && E * Y_1 + Y_1 >= 1 + S && S >= E * Y_1 && D_ >= Y_2 && E * Y_2 + Y_2 >= 1 + R + E && R + E >= E * Y_2 && Y_3 >= D_ && E * Y_3 + Y_3 >= 1 + R + E && R + E >= E * Y_3 && S_ >= Y_4 && R * Y_4 + E * Y_4 + Y_4 >= 1 + S && S >= R * Y_4 + E * Y_4 && Y_5 >= S_ && R * Y_5 + E * Y_5 + Y_5 >= 1 + S && S >= R * Y_5 + E * Y_5 && V_ >= Y_6 && E * Y_6 + Y_6 >= 1 + D1 && D1 >= E * Y_6 && Y_7 >= V_ && E * Y_7 + Y_7 >= 1 + D1 && D1 >= E * Y_7 && D1_ >= Y_8 && R * Y_8 + E * Y_8 + Y_8 >= 1 + D1 && D1 >= R * Y_8 + E * Y_8 && Y_9 >= D1_ && R * Y_9 + E * Y_9 + Y_9 >= 1 + D1 && D1 >= R * Y_9 + E * Y_9 && C1 >= 1 + Q1) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int R_ = R + E;
    if (O_ >= Y_0 && E * Y_0 + Y_0 >= 1 + S && S >= E * Y_0 && Y_1 >= O_ && E * Y_1 + Y_1 >= 1 + S && S >= E * Y_1 && D_ >= Y_2 && E * Y_2 + Y_2 >= 1 + R + E && R + E >= E * Y_2 && Y_3 >= D_ && E * Y_3 + Y_3 >= 1 + R + E && R + E >= E * Y_3 && S_ >= Y_4 && R * Y_4 + E * Y_4 + Y_4 >= 1 + S && S >= R * Y_4 + E * Y_4 && Y_5 >= S_ && R * Y_5 + E * Y_5 + Y_5 >= 1 + S && S >= R * Y_5 + E * Y_5 && V_ >= Y_6 && E * Y_6 + Y_6 >= 1 + D1 && D1 >= E * Y_6 && Y_7 >= V_ && E * Y_7 + Y_7 >= 1 + D1 && D1 >= E * Y_7 && D1_ >= Y_8 && R * Y_8 + E * Y_8 + Y_8 >= 1 + D1 && D1 >= R * Y_8 + E * Y_8 && Y_9 >= D1_ && R * Y_9 + E * Y_9 + Y_9 >= 1 + D1 && D1 >= R * Y_9 + E * Y_9 && Q1 >= 1 + C1) {
     D = D_;
     O = O_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
     goto loc_f167;
    }
   }
  goto end;
 }
 loc_f167:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Q1_ = -1 + A;
    int R_ = Y_0 + S * Y_1;
    int I_ = 1 + I;
    if (1 + Q1 == A && A >= I) {
     I = I_;
     R = R_;
     Q1 = Q1_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int R_ = Y_0 + S * Y_1 + D1 * Y_2;
    int I_ = 1 + I;
    if (A >= I && A >= 2 + Q1) {
     I = I_;
     R = R_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int R_ = Y_0 + S * Y_1 + D1 * Y_2;
    int I_ = 1 + I;
    if (A >= I && Q1 >= A) {
     I = I_;
     R = R_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int Y1_ = A;
    if (2 + Q1 >= A && I >= 1 + A) {
     Y1 = Y1_;
     goto loc_f181;
    }
   }
   if (nondet_bool()) {
    int Y1_ = 3 + Q1;
    if (A >= 3 + Q1 && I >= 1 + A) {
     Y1 = Y1_;
     goto loc_f181;
    }
   }
  goto end;
 }
 loc_f18:
 {
 __cost++;
   if (nondet_bool()) {
    int C_ = 0;
    if (A >= 1) {
     C = C_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (0 >= A) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f181:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Q1_ = -1 + A;
    int R_ = D * Y_0 + O * Y_1;
    int H_ = 1 + H;
    if (1 + Q1 == A && Y1 >= H) {
     H = H_;
     R = R_;
     Q1 = Q1_;
     goto loc_f181;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int R_ = D * Y_0 + O * Y_1 + V * Y_2;
    int H_ = 1 + H;
    if (Y1 >= H && A >= 2 + Q1) {
     H = H_;
     R = R_;
     goto loc_f181;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int R_ = D * Y_0 + O * Y_1 + V * Y_2;
    int H_ = 1 + H;
    if (Y1 >= H && Q1 >= A) {
     H = H_;
     R = R_;
     goto loc_f181;
    }
   }
   if (nondet_bool()) {
    int Q1_ = 1 + Q1;
    if (H >= 1 + Y1) {
     Q1 = Q1_;
     goto loc_f132;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int F_ = 0;
    if (1 >= 0) {
     F = F_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f23:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= B) {
     goto loc_f34;
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
     goto loc_f31;
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
     goto loc_f31;
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
     goto loc_f31;
    }
   }
  goto end;
 }
 loc_f31:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f34;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (0 >= 1 + M) {
     B = B_;
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (M >= 1) {
     B = B_;
     goto loc_f23;
    }
   }
  goto end;
 }
 loc_f34:
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
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int P_ = Y_0 * Y_1;
    if (A >= 1 + B) {
     D = D_;
     O = O_;
     P = P_;
     goto loc_f44;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int O_ = nondet();
    int D_ = nondet();
    int P_ = Y_0 * Y_1;
    if (B >= 1 + A) {
     D = D_;
     O = O_;
     P = P_;
     goto loc_f44;
    }
   }
  goto end;
 }
 loc_f4:
 {
 __cost++;
   if (nondet_bool()) {
    if (G >= H) {
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int Q_ = 0;
    int A_ = G;
    if (H >= 1 + G) {
     A = A_;
     Q = Q_;
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f41:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2 + B && 30 >= C) {
     goto loc_f23;
    }
   }
   if (nondet_bool()) {
    if (A >= 2 + B && C >= 31) {
     goto loc_f18;
    }
   }
   if (nondet_bool()) {
    if (1 + B >= A) {
     goto loc_f18;
    }
   }
  goto end;
 }
 loc_f44:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= 2 + B) {
     goto loc_f73;
    }
   }
   if (nondet_bool()) {
    if (B >= A) {
     goto loc_f73;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    int W_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int Y_ = -D + Q + 2 * O + X_;
    int V_ = 2 * O + -2 * D + X_;
    int S_ = 4 * O*O + -8 * O * D + 4 * D*D + P;
    int R_ = 2 * O + -2 * D;
    int J_ = 2 * O + -2 * D;
    int D_ = D + Q;
    int B_ = -1 + A;
    if (1 + B == A && 2 * O >= 2 * D && 4 * O*O + 4 * D*D + P >= 8 * O * D) {
     B = B_;
     D = D_;
     J = J_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     X = X_;
     Y = Y_;
     goto loc_f61;
    }
   }
   if (nondet_bool()) {
    int Z_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int Y_ = -D + Q + 2 * O + -Z_;
    int X_ = -Z_;
    int V_ = 2 * O + -2 * D + -Z_;
    int S_ = 4 * O*O + -8 * O * D + 4 * D*D + P;
    int R_ = 2 * O + -2 * D;
    int J_ = 2 * O + -2 * D;
    int D_ = D + Q;
    int B_ = -1 + A;
    if (1 + B == A && 2 * D >= 1 + 2 * O && 4 * O*O + 4 * D*D + P >= 8 * O * D) {
     B = B_;
     D = D_;
     J = J_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     X = X_;
     Y = Y_;
     Z = Z_;
     goto loc_f61;
    }
   }
   if (nondet_bool()) {
    int V_ = nondet();
    int U_ = nondet();
    int T_ = nondet();
    int J_ = nondet();
    int B1_ = -D + Q + 2 * O;
    int S_ = 4 * O*O + -8 * O * D + 4 * D*D + P;
    int R_ = 2 * O + -2 * D;
    int D_ = D + Q;
    int B_ = -1 + A;
    int A_ = -2 + A;
    if (1 + B == A && 8 * O * D >= 1 + 4 * O*O + 4 * D*D + P) {
     A = A_;
     B = B_;
     D = D_;
     J = J_;
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     B1 = B1_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f61:
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
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int A1_ = 0;
    int A_ = -2 + A;
    if (0 >= 1 + V) {
     A = A_;
     A1 = A1_;
     goto loc_f41;
    }
   }
   if (nondet_bool()) {
    int A1_ = 0;
    int A_ = -2 + A;
    if (V >= 1) {
     A = A_;
     A1 = A1_;
     goto loc_f41;
    }
   }
  goto end;
 }
 loc_f7:
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
     goto loc_f7;
    }
   }
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (I >= 1 + G) {
     H = H_;
     goto loc_f4;
    }
   }
  goto end;
 }
 loc_f73:
 {
 __cost++;
   if (nondet_bool()) {
    if (29 >= C) {
     goto loc_f75;
    }
   }
   if (nondet_bool()) {
    if (C >= 31) {
     goto loc_f75;
    }
   }
   if (nondet_bool()) {
    int C_ = 30;
    if (C == 30) {
     C = C_;
     goto loc_f75;
    }
   }
  goto end;
 }
 loc_f75:
 {
 __cost++;
   if (nondet_bool()) {
    if (9 >= C) {
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    if (C >= 11) {
     goto loc_f77;
    }
   }
   if (nondet_bool()) {
    int Q_ = Q + D;
    int C_ = 10;
    if (C == 10) {
     C = C_;
     Q = Q_;
     goto loc_f80;
    }
   }
  goto end;
 }
 loc_f77:
 {
 __cost++;
   if (nondet_bool()) {
    int Q_ = Q + D;
    int C_ = 20;
    if (C == 20) {
     C = C_;
     Q = Q_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (19 >= C) {
     C = C_;
     goto loc_f93;
    }
   }
   if (nondet_bool()) {
    int C_ = 1 + C;
    if (C >= 21) {
     C = C_;
     goto loc_f93;
    }
   }
  goto end;
 }
 loc_f80:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 1 + H;
    if (A >= H) {
     H = H_;
     goto loc_f80;
    }
   }
   if (nondet_bool()) {
    int A2_ = nondet();
    int P_ = nondet();
    int J_ = nondet();
    int Z1_ = -A2_ + 4 * J_;
    int O_ = 3 * J_;
    int E_ = 4 * J_;
    int D_ = 3 * J_;
    int C_ = 1 + C;
    if (H >= 1 + A) {
     C = C_;
     D = D_;
     E = E_;
     J = J_;
     O = O_;
     P = P_;
     Z1 = Z1_;
     A2 = A2_;
     goto loc_f93;
    }
   }
  goto end;
 }
 loc_f93:
 {
 __cost++;
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int Y_10 = nondet();
    int Y_11 = nondet();
    int O1_ = nondet();
    int N1_ = nondet();
    int M1_ = nondet();
    int L1_ = nondet();
    int J1_ = nondet();
    int I1_ = nondet();
    int H1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int P1_ = L1_ * M1_ + L1_ * N1_ + L1_ * O1_;
    int K1_ = H1_ * I1_ + H1_ * J1_;
    int E_ = E1_ + F1_ + G1_;
    if (S_ >= Y_0 && E1_ * Y_0 + F1_ * Y_0 + G1_ * Y_0 + Y_0 + D + O >= 1 + Y_1 + V_ && Y_1 + V_ >= D + O + E1_ * Y_0 + F1_ * Y_0 + G1_ * Y_0 && Y_2 >= S_ && E1_ * Y_2 + F1_ * Y_2 + G1_ * Y_2 + Y_2 + D + O >= 1 + Y_1 + V_ && Y_1 + V_ >= D + O + E1_ * Y_2 + F1_ * Y_2 + G1_ * Y_2 && R_ >= Y_3 && E1_ * Y_3 + F1_ * Y_3 + G1_ * Y_3 + Y_3 >= 1 + Y_4 + Y_5 && Y_4 + Y_5 >= E1_ * Y_3 + F1_ * Y_3 + G1_ * Y_3 && Y_4 * Y_6 + Y_4 + V_ * D + O * V_ + P >= 1 + O * D + V_*V_ && O * D + V_*V_ >= V_ * D + O * V_ + P + Y_4 * Y_6 && Y_7 >= R_ && E1_ * Y_7 + F1_ * Y_7 + G1_ * Y_7 + Y_7 >= 1 + Y_8 + Y_5 && Y_8 + Y_5 >= E1_ * Y_7 + F1_ * Y_7 + G1_ * Y_7 && Y_6 * Y_8 + Y_8 + V_ * D + O * V_ + P >= 1 + O * D + V_*V_ && O * D + V_*V_ >= V_ * D + O * V_ + P + Y_6 * Y_8 && D1_ >= Y_9 && E1_ * Y_9 + F1_ * Y_9 + G1_ * Y_9 + Y_9 >= 1 + Y_10 && Y_10 >= E1_ * Y_9 + F1_ * Y_9 + G1_ * Y_9 && Y_11 >= D1_ && E1_ * Y_11 + F1_ * Y_11 + G1_ * Y_11 + Y_11 >= 1 + Y_10 && Y_10 >= E1_ * Y_11 + F1_ * Y_11 + G1_ * Y_11 && C1 >= B && B >= 1 + C1) {
     E = E_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
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
     goto loc_f119;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int Y_10 = nondet();
    int Y_11 = nondet();
    int O1_ = nondet();
    int N1_ = nondet();
    int M1_ = nondet();
    int L1_ = nondet();
    int J1_ = nondet();
    int I1_ = nondet();
    int H1_ = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int P1_ = L1_ * M1_ + L1_ * N1_ + L1_ * O1_;
    int K1_ = H1_ * I1_ + H1_ * J1_;
    int E_ = E1_ + F1_ + G1_;
    if (C1 >= 1 + B && R_ >= Y_0 && E1_ * Y_0 + F1_ * Y_0 + G1_ * Y_0 + Y_0 >= 1 + Y_1 + Y_2 && Y_1 + Y_2 >= E1_ * Y_0 + F1_ * Y_0 + G1_ * Y_0 && Y_1 * Y_3 + Y_1 + V_ * D + O * V_ + P >= 1 + O * D + V_*V_ && O * D + V_*V_ >= V_ * D + O * V_ + P + Y_1 * Y_3 && Y_4 >= R_ && E1_ * Y_4 + F1_ * Y_4 + G1_ * Y_4 + Y_4 >= 1 + Y_5 + Y_2 && Y_5 + Y_2 >= E1_ * Y_4 + F1_ * Y_4 + G1_ * Y_4 && Y_3 * Y_5 + Y_5 + V_ * D + O * V_ + P >= 1 + O * D + V_*V_ && O * D + V_*V_ >= V_ * D + O * V_ + P + Y_3 * Y_5 && S_ >= Y_6 && E1_ * Y_6 + F1_ * Y_6 + G1_ * Y_6 + Y_6 + D + O >= 1 + Y_7 + V_ && Y_7 + V_ >= D + O + E1_ * Y_6 + F1_ * Y_6 + G1_ * Y_6 && Y_8 >= S_ && E1_ * Y_8 + F1_ * Y_8 + G1_ * Y_8 + Y_8 + D + O >= 1 + Y_7 + V_ && Y_7 + V_ >= D + O + E1_ * Y_8 + F1_ * Y_8 + G1_ * Y_8 && D1_ >= Y_9 && E1_ * Y_9 + F1_ * Y_9 + G1_ * Y_9 + Y_9 >= 1 + Y_10 && Y_10 >= E1_ * Y_9 + F1_ * Y_9 + G1_ * Y_9 && Y_11 >= D1_ && E1_ * Y_11 + F1_ * Y_11 + G1_ * Y_11 + Y_11 >= 1 + Y_10 && Y_10 >= E1_ * Y_11 + F1_ * Y_11 + G1_ * Y_11) {
     E = E_;
     R = R_;
     S = S_;
     V = V_;
     D1 = D1_;
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
     goto loc_f119;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + C1) {
     goto loc_f124;
    }
   }
   if (nondet_bool()) {
    int Y_0 = nondet();
    int Y_1 = nondet();
    int Y_2 = nondet();
    int Y_3 = nondet();
    int Y_4 = nondet();
    int Y_5 = nondet();
    int Y_6 = nondet();
    int Y_7 = nondet();
    int Y_8 = nondet();
    int Y_9 = nondet();
    int Y_10 = nondet();
    int Y_11 = nondet();
    int G1_ = nondet();
    int F1_ = nondet();
    int E1_ = nondet();
    int D1_ = nondet();
    int V_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int C1_ = B;
    int E_ = E1_ + F1_ + G1_;
    if (B == C1 && S_ >= Y_0 && E1_ * Y_0 + F1_ * Y_0 + G1_ * Y_0 + Y_0 + D + O >= 1 + Y_1 + V_ && Y_1 + V_ >= D + O + E1_ * Y_0 + F1_ * Y_0 + G1_ * Y_0 && Y_2 >= S_ && E1_ * Y_2 + F1_ * Y_2 + G1_ * Y_2 + Y_2 + D + O >= 1 + Y_1 + V_ && Y_1 + V_ >= D + O + E1_ * Y_2 + F1_ * Y_2 + G1_ * Y_2 && R_ >= Y_3 && E1_ * Y_3 + F1_ * Y_3 + G1_ * Y_3 + Y_3 >= 1 + Y_4 + Y_5 && Y_4 + Y_5 >= E1_ * Y_3 + F1_ * Y_3 + G1_ * Y_3 && Y_4 * Y_6 + Y_4 + V_ * D + O * V_ + P >= 1 + O * D + V_*V_ && O * D + V_*V_ >= V_ * D + O * V_ + P + Y_4 * Y_6 && Y_7 >= R_ && E1_ * Y_7 + F1_ * Y_7 + G1_ * Y_7 + Y_7 >= 1 + Y_8 + Y_5 && Y_8 + Y_5 >= E1_ * Y_7 + F1_ * Y_7 + G1_ * Y_7 && Y_6 * Y_8 + Y_8 + V_ * D + O * V_ + P >= 1 + O * D + V_*V_ && O * D + V_*V_ >= V_ * D + O * V_ + P + Y_6 * Y_8 && D1_ >= Y_9 && E1_ * Y_9 + F1_ * Y_9 + G1_ * Y_9 + Y_9 >= 1 + Y_10 && Y_10 >= E1_ * Y_9 + F1_ * Y_9 + G1_ * Y_9 && Y_11 >= D1_ && E1_ * Y_11 + F1_ * Y_11 + G1_ * Y_11 + Y_11 >= 1 + Y_10 && Y_10 >= E1_ * Y_11 + F1_ * Y_11 + G1_ * Y_11) {
     E = E_;
     R = R_;
     S = S_;
     V = V_;
     C1 = C1_;
     D1 = D1_;
     E1 = E1_;
     F1 = F1_;
     G1 = G1_;
     goto loc_f124;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
