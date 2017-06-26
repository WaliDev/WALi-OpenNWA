int nondet() { int a; return a; }
int nondet_bool() { int a; return a; }
int __cost;
int foo (int A, int B, int C, int D, int E, int F, int G, int H, int I, int J, int K, int L, int M, int N, int O, int P, int Q, int R, int S, int T, int U, int V, int W, int X, int Y, int Z, int A1, int B1, int C1) {
 goto loc_start;

 loc_f100:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f103;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + Z && B >= 1 + A) {
     goto loc_f152;
    }
   }
   if (nondet_bool()) {
    if (Z >= 1 && B >= 1 + A) {
     goto loc_f152;
    }
   }
   if (nondet_bool()) {
    int Z_ = 0;
    if (Z == 0 && B >= 1 + A) {
     Z = Z_;
     goto loc_f119;
    }
   }
  goto end;
 }
 loc_f103:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f103;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int Z_ = 0;
    int B_ = 1 + B;
    if (A1_ >= 0 && K >= 1 + A) {
     B = B_;
     Z = Z_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f100;
    }
   }
   if (nondet_bool()) {
    int C1_ = nondet();
    int B1_ = nondet();
    int A1_ = nondet();
    int B_ = 1 + B;
    if (0 >= 1 + A1_ && K >= 1 + A) {
     B = B_;
     A1 = A1_;
     B1 = B1_;
     C1 = C1_;
     goto loc_f100;
    }
   }
  goto end;
 }
 loc_f119:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f122;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f131;
    }
   }
  goto end;
 }
 loc_f122:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f122;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f119;
    }
   }
  goto end;
 }
 loc_f131:
 {
 __cost++;
   if (nondet_bool()) {
    int N_ = 0;
    int F_ = 0;
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     F = F_;
     N = N_;
     goto loc_f131;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int F_ = nondet();
    int O_ = O + N_;
    int B_ = 1 + B;
    if (0 >= 1 + F_ && A >= B) {
     B = B_;
     F = F_;
     N = N_;
     O = O_;
     goto loc_f131;
    }
   }
   if (nondet_bool()) {
    int N_ = nondet();
    int F_ = nondet();
    int O_ = O + N_;
    int B_ = 1 + B;
    if (F_ >= 1 && A >= B) {
     B = B_;
     F = F_;
     N = N_;
     O = O_;
     goto loc_f131;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f141;
    }
   }
  goto end;
 }
 loc_f141:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f141;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f147;
    }
   }
  goto end;
 }
 loc_f147:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f147;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f152;
    }
   }
  goto end;
 }
 loc_f15:
 {
 __cost++;
   if (nondet_bool()) {
    int G_ = 0;
    int F_ = 0;
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     F = F_;
     G = G_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int H_ = H + G_;
    int B_ = 1 + B;
    if (0 >= 1 + F_ && A >= B) {
     B = B_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int G_ = nondet();
    int F_ = nondet();
    int H_ = H + G_;
    int B_ = 1 + B;
    if (F_ >= 1 && A >= B) {
     B = B_;
     F = F_;
     G = G_;
     H = H_;
     goto loc_f15;
    }
   }
   if (nondet_bool()) {
    int I_ = 1;
    if (B >= 1 + A) {
     I = I_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f152:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f155;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f164;
    }
   }
  goto end;
 }
 loc_f155:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f155;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f152;
    }
   }
  goto end;
 }
 loc_f164:
 {
 __cost++;
   if (nondet_bool()) {
    if (B >= 1) {
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    if (0 >= B) {
     goto loc_f176;
    }
   }
  goto end;
 }
 loc_f167:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (B >= K) {
     H = H_;
     K = K_;
     goto loc_f167;
    }
   }
   if (nondet_bool()) {
    int B_ = -1 + B;
    if (K >= 1 + B) {
     B = B_;
     goto loc_f164;
    }
   }
  goto end;
 }
 loc_f176:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f176;
    }
   }
   if (nondet_bool()) {
    int Y_ = nondet();
    if (B >= 1 + A) {
     Y = Y_;
     goto loc_f184;
    }
   }
  goto end;
 }
 loc_f184:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f187;
    }
   }
   if (nondet_bool()) {
    int C_ = 0;
    if (B >= 1 + A) {
     C = C_;
     goto loc_f199;
    }
   }
  goto end;
 }
 loc_f187:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f187;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f184;
    }
   }
  goto end;
 }
 loc_f199:
 {
 __cost++;
   if (nondet_bool()) {
    int P_ = nondet();
    int B_ = 1 + B;
    if (C >= P_ && A >= B) {
     B = B_;
     P = P_;
     goto loc_f199;
    }
   }
   if (nondet_bool()) {
    int Q_ = nondet();
    int P_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (P_ >= 1 + C && A >= B) {
     B = B_;
     C = C_;
     P = P_;
     Q = Q_;
     goto loc_f199;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    if (0 >= 1 + X_ && B >= 1 + A) {
     X = X_;
     goto loc_f210;
    }
   }
   if (nondet_bool()) {
    int X_ = nondet();
    if (X_ >= 1 && B >= 1 + A) {
     X = X_;
     goto loc_f210;
    }
   }
   if (nondet_bool()) {
    int X_ = 0;
    int I_ = 0;
    int C_ = 0;
    if (B >= 1 + A) {
     C = C_;
     I = I_;
     X = X_;
     goto loc_f218;
    }
   }
  goto end;
 }
 loc_f2:
 {
 __cost++;
   if (nondet_bool()) {
    int D_ = nondet();
    int B_ = 1 + B;
    if (C >= D_ && A >= B) {
     B = B_;
     D = D_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    int E_ = nondet();
    int D_ = nondet();
    int C_ = nondet();
    int B_ = 1 + B;
    if (D_ >= 1 + C && A >= B) {
     B = B_;
     C = C_;
     D = D_;
     E = E_;
     goto loc_f2;
    }
   }
   if (nondet_bool()) {
    if (0 >= C && B >= 1 + A) {
     goto loc_f1;
    }
   }
   if (nondet_bool()) {
    if (C >= 1 && B >= 1 + A) {
     goto loc_f15;
    }
   }
  goto end;
 }
 loc_f210:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f210;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (B >= 1 + A) {
     J = J_;
     goto loc_f26;
    }
   }
  goto end;
 }
 loc_f218:
 {
 __cost++;
   if (nondet_bool()) {
    int W_ = nondet();
    int V_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int U_ = 1;
    if (V_ >= 2 && A >= B) {
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f228;
    }
   }
   if (nondet_bool()) {
    int W_ = nondet();
    int T_ = nondet();
    int S_ = nondet();
    int R_ = nondet();
    int V_ = 1;
    int U_ = 1;
    if (1 >= T_ && A >= B) {
     R = R_;
     S = S_;
     T = T_;
     U = U_;
     V = V_;
     W = W_;
     goto loc_f228;
    }
   }
   if (nondet_bool()) {
    int J_ = 1 + J;
    if (C >= 0 && B >= 1 + A) {
     J = J_;
     goto loc_f26;
    }
   }
   if (nondet_bool()) {
    if (0 >= 1 + C && B >= 1 + A) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f228:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (C >= W) {
     B = B_;
     goto loc_f218;
    }
   }
   if (nondet_bool()) {
    int C_ = W;
    int B_ = 1 + B;
    if (W >= 1 + C) {
     B = B_;
     C = C_;
     goto loc_f218;
    }
   }
  goto end;
 }
 loc_f26:
 {
 __cost++;
   if (nondet_bool()) {
    if (200 >= J && 0 >= 1 + I) {
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    if (200 >= J && I >= 1) {
     goto loc_f30;
    }
   }
   if (nondet_bool()) {
    int I_ = 0;
    if (I == 0 && 200 >= J) {
     I = I_;
     goto loc_f81;
    }
   }
   if (nondet_bool()) {
    if (J >= 201) {
     goto loc_f1;
    }
   }
  goto end;
 }
 loc_f30:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f42;
    }
   }
  goto end;
 }
 loc_f33:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (A >= K) {
     K = K_;
     goto loc_f33;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f30;
    }
   }
  goto end;
 }
 loc_f42:
 {
 __cost++;
   if (nondet_bool()) {
    int M_ = nondet();
    if (0 >= 1 + M_ && A >= 1 + L) {
     M = M_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int M_ = nondet();
    if (M_ >= 1 && A >= 1 + L) {
     M = M_;
     goto loc_f47;
    }
   }
   if (nondet_bool()) {
    int M_ = 0;
    int L_ = 1 + L;
    if (A >= 1 + L) {
     L = L_;
     M = M_;
     goto loc_f42;
    }
   }
   if (nondet_bool()) {
    if (L >= A) {
     goto loc_f68;
    }
   }
  goto end;
 }
 loc_f47:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = 0;
    if (A >= K) {
     H = H_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int L_ = 1 + L;
    if (K >= 1 + A) {
     L = L_;
     goto loc_f42;
    }
   }
  goto end;
 }
 loc_f51:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     H = H_;
     goto loc_f51;
    }
   }
   if (nondet_bool()) {
    int H_ = nondet();
    if (B >= 1 + A) {
     H = H_;
     goto loc_f58;
    }
   }
  goto end;
 }
 loc_f58:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f58;
    }
   }
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (B >= 1 + A) {
     K = K_;
     goto loc_f47;
    }
   }
  goto end;
 }
 loc_f68:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f72;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f152;
    }
   }
  goto end;
 }
 loc_f72:
 {
 __cost++;
   if (nondet_bool()) {
    int K_ = 1 + K;
    if (B >= 1 + K) {
     K = K_;
     goto loc_f72;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= B) {
     B = B_;
     goto loc_f68;
    }
   }
  goto end;
 }
 loc_f81:
 {
 __cost++;
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (A >= B) {
     B = B_;
     goto loc_f81;
    }
   }
   if (nondet_bool()) {
    if (B >= 1 + A) {
     goto loc_f87;
    }
   }
  goto end;
 }
 loc_f87:
 {
 __cost++;
   if (nondet_bool()) {
    if (A >= B) {
     goto loc_f90;
    }
   }
   if (nondet_bool()) {
    int Z_ = 1;
    if (B >= 1 + A) {
     Z = Z_;
     goto loc_f100;
    }
   }
  goto end;
 }
 loc_f90:
 {
 __cost++;
   if (nondet_bool()) {
    int H_ = nondet();
    int K_ = 1 + K;
    if (A >= K) {
     H = H_;
     K = K_;
     goto loc_f90;
    }
   }
   if (nondet_bool()) {
    int B_ = 1 + B;
    if (K >= 1 + A) {
     B = B_;
     goto loc_f87;
    }
   }
  goto end;
 }
 loc_start:
 {
 __cost++;
   if (nondet_bool()) {
    if (1 >= 0) {
     goto loc_f2;
    }
   }
  goto end;
 }
 loc_f1:
 end: __VERIFIER_print_hull(__cost);
 return 0;
}
void main() {
  foo(nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet(), nondet());
}
